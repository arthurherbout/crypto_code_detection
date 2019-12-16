/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright (c) 2009, 2010, Oracle and/or its affiliates. All rights reserved.
 * Copyright 2018 Nexenta Systems, Inc.  All rights reserved.
 */

/*
 * Support for SMB2 "signing" (message integrity)
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/proc.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/md4.h>
#include <sys/md5.h>
#include <sys/des.h>
#include <sys/kmem.h>
#include <sys/cmn_err.h>
#include <sys/stream.h>
#include <sys/strsun.h>
#include <sys/sdt.h>

#include <netsmb/smb_osdep.h>
#include <netsmb/smb2.h>
#include <netsmb/smb_conn.h>
#include <netsmb/smb_subr.h>
#include <netsmb/smb_dev.h>
#include <netsmb/smb_rq.h>
#include <netsmb/smb_signing.h>

#define	SMB2_SIG_OFF	48
#define	SMB2_SIG_LEN	16

typedef struct smb_mac_ops {
	int (*mac_init)(smb_sign_ctx_t *, smb_sign_mech_t *,
			uint8_t *, size_t);
	int (*mac_update)(smb_sign_ctx_t, uint8_t *, size_t);
	int (*mac_final)(smb_sign_ctx_t, uint8_t *);
} smb_mac_ops_t;

static smb_mac_ops_t
smb2_sign_ops = {
	smb2_hmac_init,
	smb2_hmac_update,
	smb2_hmac_final
};

static struct smb_mac_ops
smb3_sign_ops = {
	smb3_cmac_init,
	smb3_cmac_update,
	smb3_cmac_final
};

/*
 * Input to KDF for SigningKey.
 * See comment for smb3_do_kdf for content.
 */
static uint8_t sign_kdf_input[29] = {
	0, 0, 0, 1, 'S', 'M', 'B', '2',
	'A', 'E', 'S', 'C', 'M', 'A', 'C', 0,
	0, 'S', 'm', 'b', 'S', 'i', 'g', 'n',
	0, 0, 0, 0, 0x80 };

int smb3_do_kdf(void *outbuf, size_t outbuf_len,
    void *input, size_t input_len,
    uint8_t *key, uint32_t key_len);

/*
 * smb2_sign_init
 *
 * Get the mechanism info and initilize SMB2 or SMB3 signing.
 */
int
smb2_sign_init(smb_vc_t *vcp)
{
	uint_t copysize;
	int rc;

	ASSERT(vcp->vc_ssnkey != NULL);
	ASSERT(vcp->vc_mackey == NULL);

	if (SMB_DIALECT(vcp) < SMB2_DIALECT_0300)
		rc = smb2_hmac_getmech(&vcp->vc_signmech);
	else
		rc = smb3_cmac_getmech(&vcp->vc_signmech);
	if (rc != 0)
		return (EAUTH);

	/*
	 * Convert the session key to the MAC key.
	 *
	 * For SMB2, the signing key is just the first 16 bytes
	 * of the session key (truncated or padded with zeros).
	 * For SMB3, the signing key is a "KDF" hash of the
	 * session key.   [MS-SMB2] 3.2.5.3.1
	 */
	vcp->vc_mackeylen = SMB2_SIG_LEN;
	vcp->vc_mackey = kmem_zalloc(vcp->vc_mackeylen, KM_SLEEP);
	if (SMB_DIALECT(vcp) < SMB2_DIALECT_0300) {
		copysize = vcp->vc_ssnkeylen;
		if (copysize > vcp->vc_mackeylen)
			copysize = vcp->vc_mackeylen;
		bcopy(vcp->vc_ssnkey, vcp->vc_mackey, copysize);

		vcp->vc_sign_ops = &smb2_sign_ops;
	} else {
		rc = smb3_do_kdf(vcp->vc_mackey, vcp->vc_mackeylen,
		    sign_kdf_input, sizeof (sign_kdf_input),
		    vcp->vc_ssnkey, vcp->vc_ssnkeylen);
		if (rc != 0)
			return (EAUTH);
		vcp->vc_sign_ops = &smb3_sign_ops;
	}

	return (0);
}

/*
 * Derive SMB3 key as described in [MS-SMB2] 3.1.4.2
 * and [NIST SP800-108]
 *
 * r = 32, L = 128, PRF = HMAC-SHA256, key = (session key)
 *
 * Note that these describe pre-3.1.1 inputs.
 *
 * Session.SigningKey for binding a session:
 * - Session.SessionKey as K1
 * - label = SMB2AESCMAC (size 12)
 * - context = SmbSign (size 8)
 * Channel.SigningKey for for all other requests
 * - if SMB2_SESSION_FLAG_BINDING, GSS key (in Session.SessionKey?) as K1;
 * - otherwise, Session.SessionKey as K1
 * - label = SMB2AESCMAC (size 12)
 * - context = SmbSign (size 8)
 * Session.ApplicationKey for ... (not sure what yet)
 * - Session.SessionKey as K1
 * - label = SMB2APP (size 8)
 * - context = SmbRpc (size 7)
 * Session.EncryptionKey for encrypting server messages
 * - Session.SessionKey as K1
 * - label = "SMB2AESCCM" (size 11)
 * - context = "ServerOut" (size 10)
 * Session.DecryptionKey for decrypting client requests
 * - Session.SessionKey as K1
 * - label = "SMB2AESCCM" (size 11)
 * - context = "ServerIn " (size 10) (Note the space)
 */
int
smb3_do_kdf(void *outbuf, size_t outbuf_len,
    void *input, size_t input_len,
    uint8_t *key, uint32_t key_len)
{
	uint8_t digest32[SHA256_DIGEST_LENGTH];
	smb_sign_mech_t mech;
	smb_sign_ctx_t hctx = 0;
	int rc;

	bzero(&mech, sizeof (mech));
	if ((rc = smb2_hmac_getmech(&mech)) != 0)
		return (rc);

	/* Limit the SessionKey input to its maximum size (16 bytes) */
	if (key_len > SMB2_SIG_SIZE)
		key_len = SMB2_SIG_SIZE;
	rc = smb2_hmac_init(&hctx, &mech, key, key_len);
	if (rc != 0)
		return (rc);

	if ((rc = smb2_hmac_update(hctx, input, input_len)) != 0)
		return (rc);

	if ((rc = smb2_hmac_final(hctx, digest32)) != 0)
		return (rc);

	/* Output is first 16 bytes of digest. */
	bcopy(digest32, outbuf, outbuf_len);
	return (0);
}

/*
 * Compute MAC signature of packet data, using the stored MAC key.
 *
 * The signature is in the last 16 bytes of the SMB2 header.
 * The signature algorighm is to compute HMAC SHA256 over the
 * entire command, with the signature field set to zeros.
 *
 * See similar code for the server side:
 * uts/common/fs/smbsrv/smb2_signing.c : smb2_sign_calc
 */
static int
smb2_compute_MAC(struct smb_vc *vcp, mblk_t *mp, uchar_t *signature)
{
	uint8_t tmp_hdr[SMB2_HDR_SIZE];
	smb_sign_ctx_t ctx = 0;
	smb_mac_ops_t *ops;
	mblk_t *m = mp;
	int size;
	int rc;

	if (vcp->vc_mackey == NULL)
		return (-1);
	if ((ops = vcp->vc_sign_ops) == NULL)
		return (-1);

	rc = ops->mac_init(&ctx, &vcp->vc_signmech,
	    vcp->vc_mackey, vcp->vc_mackeylen);
	if (rc != 0)
		return (rc);

	/* Our caller should ensure mp has a contiguous header */
	ASSERT(m != NULL);
	ASSERT(MBLKL(m) >= SMB2_HDRLEN);

	/*
	 * Copy of the SMB2 header, zero out the signature, and digest.
	 */
	size = SMB2_HDRLEN;
	bcopy(m->b_rptr, tmp_hdr, size);
	bzero(tmp_hdr + SMB2_SIG_OFF, SMB2_SIG_LEN);
	rc = ops->mac_update(ctx, tmp_hdr, size);
	if (rc != 0)
		return (rc);

	/*
	 * Digest the rest of the SMB2 header packet, starting at
	 * the data just after the SMB2 header.
	 */
	size = MBLKL(m) - SMB2_HDRLEN;
	rc = ops->mac_update(ctx, m->b_rptr + SMB2_HDRLEN, size);
	if (rc != 0)
		return (rc);
	m = m->b_cont;

	/* Digest rest of the SMB2 message. */
	while (m != NULL) {
		size = MBLKL(m);
		if (size > 0) {
			rc = ops->mac_update(ctx, m->b_rptr, size);
			if (rc != 0)
				return (rc);
		}
		m = m->b_cont;
	}
	rc = ops->mac_final(ctx, signature);

	return (rc);
}

/*
 * Sign a request with HMAC-MD5.
 */
void
smb2_rq_sign(struct smb_rq *rqp)
{
	struct smb_vc *vcp = rqp->sr_vc;
	mblk_t *mp = rqp->sr_rq.mb_top;
	uint8_t *sigloc;
	int rc;

	/*
	 * smb_rq_new() ensures this,
	 * but just in case..
	 */
	ASSERT(MBLKL(mp) >= SMB2_HDRLEN);
	sigloc = mp->b_rptr + SMB2_SIG_OFF;

	if (vcp->vc_mackey == NULL)
		return;

	/*
	 * This will compute the MAC and store it
	 * directly into the message at sigloc.
	 */
	rc = smb2_compute_MAC(vcp, mp, sigloc);
	if (rc != 0) {
		SMBSDEBUG("Crypto error %d", rc);
		bzero(sigloc, SMB2_SIG_LEN);
	}
}

/*
 * Verify reply signature.
 */
int
smb2_rq_verify(struct smb_rq *rqp)
{
	struct smb_vc *vcp = rqp->sr_vc;
	mblk_t *mp = rqp->sr_rp.md_top;
	uint8_t sigbuf[SMB2_SIG_LEN];
	uint8_t *sigloc;
	int rc;

	/*
	 * Note vc_mackey and vc_mackeylen gets filled in by
	 * smb_usr_iod_work as the connection comes in.
	 */
	if (vcp->vc_mackey == NULL) {
		SMBSDEBUG("no mac key\n");
		return (0);
	}

	/*
	 * Let caller deal with empty reply or short messages by
	 * returning zero.  Caller will fail later, in parsing.
	 */
	if (mp == NULL) {
		SMBSDEBUG("empty reply\n");
		return (0);
	}

	/* smb2_iod_process ensures this */
	ASSERT(MBLKL(mp) >= SMB2_HDRLEN);
	sigloc = mp->b_rptr + SMB2_SIG_OFF;

	/*
	 * Compute the expected signature in sigbuf.
	 */
	rc = smb2_compute_MAC(vcp, mp, sigbuf);
	if (rc != 0) {
		SMBSDEBUG("Crypto error %d", rc);
		/*
		 * If we can't compute a MAC, then there's
		 * no point trying other seqno values.
		 */
		return (EBADRPC);
	}

	/*
	 * Compare the computed signature with the
	 * one found in the message (at sigloc)
	 */
	if (bcmp(sigbuf, sigloc, SMB2_SIG_LEN) == 0)
		return (0);

	SMBERROR("BAD signature, Server=%s MID=0x%llx\n",
	    vcp->vc_srvname, (long long)rqp->sr2_messageid);

	return (EBADRPC);
}
