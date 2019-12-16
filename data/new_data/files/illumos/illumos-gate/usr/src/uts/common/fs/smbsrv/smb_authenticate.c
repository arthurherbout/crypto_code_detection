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
 * Copyright (c) 2007, 2010, Oracle and/or its affiliates. All rights reserved.
 * Copyright 2019 Nexenta by DDN, Inc. All rights reserved.
 */

/*
 * Authentication support for SMB session setup
 */

#include <sys/types.h>
#include <sys/sid.h>
#include <sys/priv_names.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <smbsrv/smb_idmap.h>
#include <smbsrv/smb_kproto.h>
#include <smbsrv/smb_token.h>

static uint32_t smb_authsock_open(smb_request_t *);
static int smb_authsock_send(ksocket_t, void *, size_t);
static int smb_authsock_recv(ksocket_t, void *, size_t);
static uint32_t smb_authsock_sendrecv(smb_request_t *, smb_lsa_msg_hdr_t *hdr,
				void *sndbuf, void **recvbuf);
/* void smb_authsock_close(smb_user_t *); kproto.h */

static uint32_t smb_auth_do_clinfo(smb_request_t *);
static uint32_t smb_auth_do_oldreq(smb_request_t *);
static uint32_t smb_auth_get_token(smb_request_t *);
static uint32_t smb_priv_xlate(smb_token_t *);

/*
 * Handle old-style session setup (non-extended security)
 * Note: Used only by SMB1
 *
 * The user information is passed to smbd for authentication.
 * If smbd can authenticate the user an access token is returned and we
 * generate a cred and new user based on the token.
 */
int
smb_authenticate_old(smb_request_t *sr)
{
	smb_user_t	*user = NULL;
	uint32_t	status;

	user = smb_user_new(sr->session);
	if (user == NULL)
		return (NT_STATUS_TOO_MANY_SESSIONS);

	/* user cleanup in smb_request_free */
	sr->uid_user = user;
	sr->smb_uid = user->u_uid;
	sr->smb2_ssnid = 0;

	/*
	 * Open a connection to the local logon service.
	 * If we can't, it may be busy, or not running.
	 * Don't log here - this may be frequent.
	 */
	if ((status = smb_authsock_open(sr)) != 0)
		goto errout;

	/*
	 * Tell the auth. svc who this client is.
	 */
	if ((status = smb_auth_do_clinfo(sr)) != 0)
		goto errout;

	/*
	 * Authentication proper
	 */
	if ((status = smb_auth_do_oldreq(sr)) != 0)
		goto errout;

	/*
	 * Get the final auth. token.
	 */
	if ((status = smb_auth_get_token(sr)) != 0)
		goto errout;

	return (0);

errout:
	smb_user_logoff(user);
	return (status);
}

/*
 * Build an authentication request message and
 * send it to the local logon service.
 */
static uint32_t
smb_auth_do_oldreq(smb_request_t *sr)
{
	smb_lsa_msg_hdr_t	msg_hdr;
	smb_logon_t	user_info;
	XDR		xdrs;
	smb_arg_sessionsetup_t *sinfo = sr->sr_ssetup;
	void		*sbuf = NULL;
	void		*rbuf = NULL;
	uint32_t	slen = 0;
	uint32_t	rlen = 0;
	uint32_t	status;
	bool_t		ok;

	bzero(&user_info, sizeof (smb_logon_t));

	user_info.lg_level = NETR_NETWORK_LOGON;
	user_info.lg_username = sinfo->ssi_user;
	user_info.lg_domain = sinfo->ssi_domain;
	user_info.lg_workstation = sr->session->workstation;
	user_info.lg_clnt_ipaddr = sr->session->ipaddr;
	user_info.lg_local_ipaddr = sr->session->local_ipaddr;
	user_info.lg_local_port = sr->session->s_local_port;
	user_info.lg_challenge_key.val = sr->session->challenge_key;
	user_info.lg_challenge_key.len = sr->session->challenge_len;
	user_info.lg_nt_password.val = sinfo->ssi_ntpwd;
	user_info.lg_nt_password.len = sinfo->ssi_ntpwlen;
	user_info.lg_lm_password.val = sinfo->ssi_lmpwd;
	user_info.lg_lm_password.len = sinfo->ssi_lmpwlen;
	user_info.lg_native_os = sr->session->native_os;
	user_info.lg_native_lm = sr->session->native_lm;
	/* lg_flags? */

	slen = xdr_sizeof(smb_logon_xdr, &user_info);
	sbuf = kmem_alloc(slen, KM_SLEEP);
	xdrmem_create(&xdrs, sbuf, slen, XDR_ENCODE);
	ok = smb_logon_xdr(&xdrs, &user_info);
	xdr_destroy(&xdrs);
	if (!ok) {
		status = RPC_NT_BAD_STUB_DATA;
		goto out;
	}

	msg_hdr.lmh_msgtype = LSA_MTYPE_OLDREQ;
	msg_hdr.lmh_msglen = slen;
	status = smb_authsock_sendrecv(sr, &msg_hdr, sbuf, &rbuf);
	if (status != 0)
		goto out;
	rlen = msg_hdr.lmh_msglen;
	kmem_free(sbuf, slen);
	sbuf = NULL;

	/*
	 * Decode the response message.
	 */
	switch (msg_hdr.lmh_msgtype) {

	case LSA_MTYPE_OK:
		status = 0;
		break;

	case LSA_MTYPE_ERROR:
		if (rlen == sizeof (smb_lsa_eresp_t)) {
			smb_lsa_eresp_t *ler = rbuf;
			status = ler->ler_ntstatus;
			break;
		}
		/* FALLTHROUGH */

	default:	/*  Bogus message type */
		status = NT_STATUS_INTERNAL_ERROR;
		break;
	}

out:
	if (rbuf != NULL)
		kmem_free(rbuf, rlen);
	if (sbuf != NULL)
		kmem_free(sbuf, slen);

	return (status);
}

/*
 * Handle new-style (extended security) session setup.
 * Returns zero: success, non-zero: error (value not used)
 *
 * Note that this style uses a sequence of session setup requests,
 * where the first has SMB UID=0, and subsequent requests in the
 * same authentication sequence have the SMB UID returned for that
 * first request.  We allocate a USER object when the first request
 * in the sequence arrives (SMB_USER_STATE_LOGGING_ON) and use that
 * to maintain state between requests in this sequence.  The state
 * for one sequence includes an AF_UNIX "authsock" connection to the
 * user-space smbd.  The neat part of this is: in smbd, the handler
 * for the server-side of one authsock gets only request specific to
 * one authentication sequence, simplifying it's work immensely.
 * When the authentication sequence is finished, with either success
 * or failure, the local side of the authsock is closed.
 *
 * As with the old-style authentication, if we succeed, then the
 * last message from smbd will be an smb_token_t encoding the
 * information about the new user.
 *
 * Outline:
 * (a) On the first request (UID==0) create a USER object,
 *     and on subsequent requests, find USER by SMB UID.
 * (b) Send message / recv. response as above,
 * (c) If response says "we're done", close authsock
 *     (both success and failure must close authsock)
 */
int
smb_authenticate_ext(smb_request_t *sr)
{
	smb_lsa_msg_hdr_t	msg_hdr;
	smb_arg_sessionsetup_t *sinfo = sr->sr_ssetup;
	smb_user_t	*user = NULL;
	void		*rbuf = NULL;
	uint32_t	rlen = 0;
	uint32_t	status;

	ASSERT(sr->uid_user == NULL);

	/*
	 * Paranoid:  While finding/creating the user object, make sure
	 * SMB2 ignores smb_uid, and SMB1 ignores smb2_ssnid.  The
	 * logic below assumes the "other" one is always zero; both
	 * the "first request" tests and smb_session_lookup_uid_st.
	 */
	if (sr->session->dialect >= SMB_VERS_2_BASE) {
		/* SMB2+ ignores smb_uid */
		ASSERT(sr->smb_uid == 0);
		sr->smb_uid = 0;
	} else {
		/* SMB1 ignores smb2_ssnid */
		ASSERT(sr->smb2_ssnid == 0);
		sr->smb2_ssnid = 0;
	}

	/*
	 * On the first request (UID/ssnid==0) create a USER object.
	 * On subsequent requests (UID/ssnid!=0) find the USER object.
	 * Either way, sr->uid_user is set, so our ref. on the
	 * user object is dropped during normal cleanup work
	 * for the smb_request (sr).  Ditto u_authsock.
	 */
	if (sr->smb2_ssnid == 0 && sr->smb_uid == 0) {
		user = smb_user_new(sr->session);
		if (user == NULL)
			return (NT_STATUS_TOO_MANY_SESSIONS);

		/* user cleanup in smb_request_free */
		sr->uid_user = user;
		if (sr->session->dialect >= SMB_VERS_2_BASE) {
			/* Intentionally leave smb_uid=0 for SMB2 */
			sr->smb2_ssnid = user->u_ssnid;
		} else {
			/* Intentionally leave smb2_ssnid=0 for SMB1 */
			sr->smb_uid = user->u_uid;
		}

		/*
		 * Open a connection to the local logon service.
		 * If we can't, it may be busy, or not running.
		 * Don't log here - this may be frequent.
		 */
		if ((status = smb_authsock_open(sr)) != 0)
			goto errout;

		/*
		 * Tell the auth. svc who this client is.
		 */
		if ((status = smb_auth_do_clinfo(sr)) != 0)
			goto errout;

		msg_hdr.lmh_msgtype = LSA_MTYPE_ESFIRST;
	} else {
		user = smb_session_lookup_uid_st(sr->session,
		    sr->smb2_ssnid, sr->smb_uid, SMB_USER_STATE_LOGGING_ON);
		if (user == NULL)
			return (NT_STATUS_USER_SESSION_DELETED);

		/* user cleanup in smb_request_free */
		sr->uid_user = user;

		msg_hdr.lmh_msgtype = LSA_MTYPE_ESNEXT;
	}

	/*
	 * Wrap the "security blob" with our header
	 * (LSA_MTYPE_ESFIRST or LSA_MTYPE_ESNEXT)
	 * and send it up the authsock with either
	 */
	msg_hdr.lmh_msglen = sinfo->ssi_iseclen;
	status = smb_authsock_sendrecv(sr, &msg_hdr,
	    sinfo->ssi_isecblob, &rbuf);
	if (status != 0)
		goto errout;
	rlen = msg_hdr.lmh_msglen;

	/*
	 * Decode the response message.
	 * Note: allocated rbuf
	 */
	switch (msg_hdr.lmh_msgtype) {

	case LSA_MTYPE_ES_CONT:
		sinfo->ssi_oseclen = (uint16_t)rlen;
		sinfo->ssi_osecblob = smb_srm_alloc(sr, sinfo->ssi_oseclen);
		bcopy(rbuf, sinfo->ssi_osecblob, sinfo->ssi_oseclen);
		/*
		 * This is not really an error, but tells the client
		 * it should send another session setup request.
		 */
		status = NT_STATUS_MORE_PROCESSING_REQUIRED;
		break;

	case LSA_MTYPE_ES_DONE:
		sinfo->ssi_oseclen = (uint16_t)rlen;
		sinfo->ssi_osecblob = smb_srm_alloc(sr, sinfo->ssi_oseclen);
		bcopy(rbuf, sinfo->ssi_osecblob, sinfo->ssi_oseclen);
		sinfo->ssi_ntpwlen = 0;
		/*
		 * Get the final auth. token.
		 */
		status = smb_auth_get_token(sr);
		break;

	case LSA_MTYPE_ERROR:
		/*
		 * Authentication failed.  Return the error
		 * provided in the reply message.
		 */
		if (rlen == sizeof (smb_lsa_eresp_t)) {
			smb_lsa_eresp_t *ler = rbuf;
			status = ler->ler_ntstatus;
			goto errout;
		}
		/* FALLTHROUGH */

	default:	/*  Bogus message type */
		status = NT_STATUS_INTERNAL_ERROR;
		goto errout;
	}

	if (status != 0 && status != NT_STATUS_MORE_PROCESSING_REQUIRED) {
	errout:
		smb_user_logoff(user);
	}

	if (rbuf != NULL)
		kmem_free(rbuf, rlen);

	return (status);
}

/*
 * Send the "client info" up to the auth service.
 */
static uint32_t
smb_auth_do_clinfo(smb_request_t *sr)
{
	smb_lsa_msg_hdr_t msg_hdr;
	smb_lsa_clinfo_t clinfo;
	void *rbuf = NULL;
	uint32_t status;

	/*
	 * Send a message with info. about the client
	 * (IP address, etc) and wait for an ACK.
	 */
	msg_hdr.lmh_msgtype = LSA_MTYPE_CLINFO;
	msg_hdr.lmh_msglen = sizeof (clinfo);
	clinfo.lci_clnt_ipaddr = sr->session->ipaddr;
	(void) memcpy(clinfo.lci_challenge_key,
	    sr->session->challenge_key,
	    sizeof (clinfo.lci_challenge_key));
	status = smb_authsock_sendrecv(sr, &msg_hdr, &clinfo, &rbuf);
	/* We don't use this response. */
	if (rbuf != NULL) {
		kmem_free(rbuf, msg_hdr.lmh_msglen);
		rbuf = NULL;
	}

	return (status);
}

/*
 * After a successful authentication, ask the authsvc to
 * send us the authentication token.
 */
static uint32_t
smb_auth_get_token(smb_request_t *sr)
{
	smb_lsa_msg_hdr_t msg_hdr;
	XDR		xdrs;
	smb_user_t	*user = sr->uid_user;
	smb_token_t	*token = NULL;
	cred_t		*cr = NULL;
	void		*rbuf = NULL;
	uint32_t	rlen = 0;
	uint32_t	privileges;
	uint32_t	status;
	bool_t		ok;

	msg_hdr.lmh_msgtype = LSA_MTYPE_GETTOK;
	msg_hdr.lmh_msglen = 0;

	status = smb_authsock_sendrecv(sr, &msg_hdr, NULL, &rbuf);
	if (status != 0)
		goto errout;

	rlen = msg_hdr.lmh_msglen;
	switch (msg_hdr.lmh_msgtype) {

	case LSA_MTYPE_TOKEN:
		status = 0;
		break;

	case LSA_MTYPE_ERROR:
		if (rlen == sizeof (smb_lsa_eresp_t)) {
			smb_lsa_eresp_t *ler = rbuf;
			status = ler->ler_ntstatus;
			goto errout;
		}
		/* FALLTHROUGH */

	default:
		status = NT_STATUS_INTERNAL_ERROR;
		goto errout;
	}

	/*
	 * Authenticated.  Decode the LSA_MTYPE_TOKEN.
	 */
	xdrmem_create(&xdrs, rbuf, rlen, XDR_DECODE);
	token = kmem_zalloc(sizeof (smb_token_t), KM_SLEEP);
	ok = smb_token_xdr(&xdrs, token);
	xdr_destroy(&xdrs);
	if (!ok) {
		status = RPC_NT_BAD_STUB_DATA;
		goto errout;
	}
	kmem_free(rbuf, rlen);
	rbuf = NULL;

	/*
	 * Setup the logon object.
	 */
	cr = smb_cred_create(token);
	if (cr == NULL)
		goto errout;
	privileges = smb_priv_xlate(token);
	(void) smb_user_logon(user, cr,
	    token->tkn_domain_name, token->tkn_account_name,
	    token->tkn_flags, privileges, token->tkn_audit_sid);
	crfree(cr);

	/*
	 * Some basic processing for encryption needs to be done,
	 * even for anonymous/guest sessions. In particular,
	 * we need to set Session.EncryptData.
	 *
	 * Windows handling of anon/guest and encryption is strange.
	 * It allows these accounts to get through session setup,
	 * even when they provide no key material.
	 * Additionally, Windows somehow manages to have key material
	 * for anonymous accounts under unknown circumstances.
	 * As such, We set EncryptData on anon/guest to behave like Windows,
	 * at least through Session Setup.
	 */
	if (sr->session->dialect >= SMB_VERS_3_0)
		smb3_encrypt_begin(sr, token);

	/*
	 * Save the session key, and (maybe) enable signing,
	 * but only for real logon (not ANON or GUEST).
	 */
	if ((token->tkn_flags & (SMB_ATF_GUEST | SMB_ATF_ANON)) == 0) {
		if (sr->session->dialect >= SMB_VERS_2_BASE) {
			smb2_sign_begin(sr, token);
		} else {
			smb_sign_begin(sr, token);
		}
	}

	smb_token_free(token);

	sr->user_cr = user->u_cred;
	return (0);

errout:
	if (rbuf != NULL)
		kmem_free(rbuf, rlen);
	if (token != NULL)
		smb_token_free(token);
	return (status);
}

/*
 * Tokens are allocated in the kernel via XDR.
 * Call xdr_free before freeing the token structure.
 */
void
smb_token_free(smb_token_t *token)
{
	if (token != NULL) {
		xdr_free(smb_token_xdr, (char *)token);
		kmem_free(token, sizeof (smb_token_t));
	}
}

/*
 * Convert access token privileges to local definitions.
 */
static uint32_t
smb_priv_xlate(smb_token_t *token)
{
	uint32_t	privileges = 0;

	if (smb_token_query_privilege(token, SE_SECURITY_LUID))
		privileges |= SMB_USER_PRIV_SECURITY;

	if (smb_token_query_privilege(token, SE_TAKE_OWNERSHIP_LUID))
		privileges |= SMB_USER_PRIV_TAKE_OWNERSHIP;

	if (smb_token_query_privilege(token, SE_BACKUP_LUID))
		privileges |= SMB_USER_PRIV_BACKUP;

	if (smb_token_query_privilege(token, SE_RESTORE_LUID))
		privileges |= SMB_USER_PRIV_RESTORE;

	if (smb_token_query_privilege(token, SE_CHANGE_NOTIFY_LUID))
		privileges |= SMB_USER_PRIV_CHANGE_NOTIFY;

	if (smb_token_query_privilege(token, SE_READ_FILE_LUID))
		privileges |= SMB_USER_PRIV_READ_FILE;

	if (smb_token_query_privilege(token, SE_WRITE_FILE_LUID))
		privileges |= SMB_USER_PRIV_WRITE_FILE;

	return (privileges);
}

/*
 * Unblock a request that might be blocked reading some
 * authentication socket.  This can happen when either the
 * client cancels a session setup or closes the connection.
 */
static void
smb_authsock_cancel(smb_request_t *sr)
{
	smb_user_t *user = sr->cancel_arg2;
	ksocket_t authsock = NULL;

	if (user == NULL)
		return;
	ASSERT(user == sr->uid_user);

	/*
	 * Check user state, and get a hold on the auth socket.
	 */
	mutex_enter(&user->u_mutex);
	if (user->u_state == SMB_USER_STATE_LOGGING_ON) {
		if ((authsock = user->u_authsock) != NULL)
			ksocket_hold(authsock);
	}
	mutex_exit(&user->u_mutex);

	if (authsock != NULL) {
		(void) ksocket_shutdown(authsock, SHUT_RDWR, sr->user_cr);
		ksocket_rele(authsock);
	}
}

/*
 * Send/recv a request/reply sequence on the auth socket.
 * Returns zero or an NT status.
 *
 * Errors here mean we can't communicate with the smbd_authsvc.
 * With limited authsock instances, this should be rare.
 */
static uint32_t
smb_authsock_sendrecv(smb_request_t *sr, smb_lsa_msg_hdr_t *hdr,
    void *sndbuf, void **recvbuf)
{
	smb_user_t *user = sr->uid_user;
	ksocket_t so;
	uint32_t status;
	int rc;

	/*
	 * Get a hold on the auth socket.
	 */
	mutex_enter(&user->u_mutex);
	so = user->u_authsock;
	if (so == NULL) {
		mutex_exit(&user->u_mutex);
		return (NT_STATUS_INTERNAL_ERROR);
	}
	ksocket_hold(so);
	mutex_exit(&user->u_mutex);

	mutex_enter(&sr->sr_mutex);
	if (sr->sr_state != SMB_REQ_STATE_ACTIVE) {
		mutex_exit(&sr->sr_mutex);
		status = NT_STATUS_CANCELLED;
		goto out;
	}
	sr->sr_state = SMB_REQ_STATE_WAITING_AUTH;
	sr->cancel_method = smb_authsock_cancel;
	sr->cancel_arg2 = user;
	mutex_exit(&sr->sr_mutex);

	rc = smb_authsock_send(so, hdr, sizeof (*hdr));
	if (rc == 0 && hdr->lmh_msglen != 0) {
		rc = smb_authsock_send(so, sndbuf, hdr->lmh_msglen);
	}
	if (rc == 0)
		rc = smb_authsock_recv(so, hdr, sizeof (*hdr));
	if (rc == 0 && hdr->lmh_msglen != 0) {
		*recvbuf = kmem_alloc(hdr->lmh_msglen, KM_SLEEP);
		rc = smb_authsock_recv(so, *recvbuf, hdr->lmh_msglen);
	}

	switch (rc) {
	case 0:
		status = 0;
		break;
	case EIO:
		status = RPC_NT_COMM_FAILURE;
		break;
	case ENOTCONN:
		status = RPC_NT_PIPE_CLOSED;
		break;
	default:
		status = RPC_NT_CALL_FAILED;
		break;
	}

	mutex_enter(&sr->sr_mutex);
	sr->cancel_method = NULL;
	sr->cancel_arg2 = NULL;
	switch (sr->sr_state) {
	case SMB_REQ_STATE_WAITING_AUTH:
		sr->sr_state = SMB_REQ_STATE_ACTIVE;
		break;
	case SMB_REQ_STATE_CANCEL_PENDING:
		sr->sr_state = SMB_REQ_STATE_CANCELLED;
		status = NT_STATUS_CANCELLED;
		break;
	default:
		status = NT_STATUS_INTERNAL_ERROR;
		break;
	}
	mutex_exit(&sr->sr_mutex);

out:
	ksocket_rele(so);

	if (status != 0 && *recvbuf != NULL) {
		kmem_free(*recvbuf, hdr->lmh_msglen);
		*recvbuf = NULL;
	}
	return (status);
}

/*
 * Hope this is interpreted per-zone...
 */
static struct sockaddr_un smbauth_sockname = {
	AF_UNIX, SMB_AUTHSVC_SOCKNAME };

/*
 * Limit how long smb_authsock_sendrecv() will wait for a
 * response from the local authentication service.
 */
struct timeval smb_auth_recv_tmo = { 45, 0 };

/*
 * Also limit the time smb_authsock_sendrecv() will wait
 * trying to send a request to the authentication service.
 */
struct timeval smb_auth_send_tmo = { 15, 0 };

/*
 * Maximum time a user object may stay in state LOGGING_ON
 */
int smb_auth_total_tmo = 45;	/* seconds */

static uint32_t
smb_authsock_open(smb_request_t *sr)
{
	smb_user_t *user = sr->uid_user;
	smb_server_t *sv = sr->sr_server;
	ksocket_t so = NULL;
	uint32_t status = 0;
	int rc;

	/*
	 * If the auth. service is busy, wait our turn.  This threshold
	 * limits the number of auth sockets we might have trying to
	 * communicate with the auth. service up in smbd.  Until we've
	 * set u_authsock, we need to "exit this threshold" in any
	 * error code paths after this "enter".
	 *
	 * Failure to "enter" may be frequent, so don't log.
	 */
	if ((rc = smb_threshold_enter(&sv->sv_ssetup_ct)) != 0)
		return (NT_STATUS_NO_LOGON_SERVERS);

	rc = ksocket_socket(&so, AF_UNIX, SOCK_STREAM, 0,
	    KSOCKET_SLEEP, CRED());
	if (rc != 0) {
		cmn_err(CE_NOTE, "smb_authsock_open: socket, rc=%d", rc);
		smb_threshold_exit(&sv->sv_ssetup_ct);
		status = NT_STATUS_INSUFF_SERVER_RESOURCES;
		goto errout;
	}

	/*
	 * This (new) user object now gets an authsocket.
	 * Note: u_authsock cleanup in smb_user_logoff.
	 * After we've set u_authsock, smb_threshold_exit
	 * is done in smb_authsock_close().  If we somehow
	 * already have an authsock, close the new one and
	 * error out.
	 */
	mutex_enter(&user->u_mutex);
	if (user->u_authsock != NULL) {
		mutex_exit(&user->u_mutex);
		smb_authsock_close(user, so);
		status = NT_STATUS_INTERNAL_ERROR;
		goto errout;
	}
	user->u_authsock = so;
	if (smb_auth_total_tmo != 0) {
		user->u_auth_tmo = timeout(smb_user_auth_tmo, user,
		    SEC_TO_TICK(smb_auth_total_tmo));
	}
	mutex_exit(&user->u_mutex);

	/*
	 * Set the send/recv timeouts.
	 */
	(void) ksocket_setsockopt(so, SOL_SOCKET, SO_SNDTIMEO,
	    &smb_auth_send_tmo, sizeof (smb_auth_send_tmo), CRED());
	(void) ksocket_setsockopt(so, SOL_SOCKET, SO_RCVTIMEO,
	    &smb_auth_recv_tmo, sizeof (smb_auth_recv_tmo), CRED());

	/*
	 * Connect to the smbd auth. service.
	 *
	 * Would like to set the connect timeout too, but there's
	 * apparently no easy way to do that for AF_UNIX.
	 */
	mutex_enter(&sr->sr_mutex);
	if (sr->sr_state != SMB_REQ_STATE_ACTIVE) {
		mutex_exit(&sr->sr_mutex);
		status = NT_STATUS_CANCELLED;
		goto errout;
	}
	sr->sr_state = SMB_REQ_STATE_WAITING_AUTH;
	sr->cancel_method = smb_authsock_cancel;
	sr->cancel_arg2 = user;
	mutex_exit(&sr->sr_mutex);

	rc = ksocket_connect(so, (struct sockaddr *)&smbauth_sockname,
	    sizeof (smbauth_sockname), CRED());
	if (rc != 0) {
		DTRACE_PROBE1(error, int, rc);
		status = NT_STATUS_NETLOGON_NOT_STARTED;
	}

	mutex_enter(&sr->sr_mutex);
	sr->cancel_method = NULL;
	sr->cancel_arg2 = NULL;
	switch (sr->sr_state) {
	case SMB_REQ_STATE_WAITING_AUTH:
		sr->sr_state = SMB_REQ_STATE_ACTIVE;
		break;
	case SMB_REQ_STATE_CANCEL_PENDING:
		sr->sr_state = SMB_REQ_STATE_CANCELLED;
		status = NT_STATUS_CANCELLED;
		break;
	default:
		status = NT_STATUS_INTERNAL_ERROR;
		break;
	}
	mutex_exit(&sr->sr_mutex);

errout:
	return (status);
}

static int
smb_authsock_send(ksocket_t so, void *buf, size_t len)
{
	int rc;
	size_t iocnt = 0;

	rc = ksocket_send(so, buf, len, 0, &iocnt, CRED());
	if (rc == 0 && iocnt != len) {
		DTRACE_PROBE1(short, size_t, iocnt);
		rc = EIO;
	}
	if (rc != 0) {
		DTRACE_PROBE1(error, int, rc);
	}

	return (rc);
}

static int
smb_authsock_recv(ksocket_t so, void *buf, size_t len)
{
	int rc;
	size_t iocnt = 0;

	rc = ksocket_recv(so, buf, len, MSG_WAITALL, &iocnt, CRED());
	if (rc == 0) {
		if (iocnt == 0) {
			DTRACE_PROBE1(discon, struct sonode *, so);
			rc = ENOTCONN;
		} else if (iocnt != len) {
			/* Should not happen with MSG_WAITALL */
			DTRACE_PROBE1(short, size_t, iocnt);
			rc = EIO;
		}
	}
	if (rc != 0) {
		DTRACE_PROBE1(error, int, rc);
	}

	return (rc);
}

/*
 * Caller has cleared user->u_authsock, passing the last ref
 * as the 2nd arg here.  This can block, so it's called
 * after exiting u_mutex.
 */
void
smb_authsock_close(smb_user_t *user, ksocket_t so)
{

	(void) ksocket_shutdown(so, SHUT_RDWR, CRED());
	(void) ksocket_close(so, CRED());
	smb_threshold_exit(&user->u_server->sv_ssetup_ct);
}
