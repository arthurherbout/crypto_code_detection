/*
 * This file and its contents are supplied under the terms of the
 * Common Development and Distribution License ("CDDL"), version 1.0.
 * You may only use this file in accordance with the terms of version
 * 1.0 of the CDDL.
 *
 * A full copy of the text of the CDDL should have accompanied this
 * source.  A copy of the CDDL is also available via the Internet at
 * http://www.illumos.org/license/CDDL.
 */

/*
 * Copyright 2015 Nexenta Systems, Inc.  All rights reserved.
 * Copyright 2019 Joyent, Inc.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <cryptoutil.h>
#include <security/cryptoki.h>
#include <sys/debug.h>

#include "cryptotest.h"

struct crypto_op {
	CK_BYTE_PTR in;
	CK_BYTE_PTR out;
	CK_BYTE_PTR key;
	CK_BYTE_PTR param;

	size_t inlen;
	size_t outlen;
	size_t keylen;
	size_t paramlen;
	const size_t *updatelens;

	char *mechname;

	/* internal */
	CK_MECHANISM_TYPE mech;
	CK_OBJECT_HANDLE keyt;
	CK_SESSION_HANDLE hsession;
	size_t fg;
};

static void
cryptotest_error(char *name, CK_RV rv)
{
	(void) fprintf(stderr, "%s: Error = 0x%.8lX '%s'\n",
	    name, rv, pkcs11_strerror(rv));
}

crypto_op_t *
cryptotest_init(cryptotest_t *arg, crypto_func_group_t fg)
{
	crypto_op_t *op = malloc(sizeof (*op));

	if (op == NULL) {
		(void) fprintf(stderr, "malloc failed: %s\n", strerror(errno));
		return (NULL);
	}

	op->in = (CK_BYTE_PTR)arg->in;
	op->out = (CK_BYTE_PTR)arg->out;
	op->key = (CK_BYTE_PTR)arg->key;
	op->param = (CK_BYTE_PTR)arg->param;

	op->inlen = arg->inlen;
	op->outlen = arg->outlen;
	op->keylen = arg->keylen;
	op->paramlen = arg->plen;
	op->updatelens = arg->updatelens;

	op->mechname = arg->mechname;

	op->hsession = CK_INVALID_HANDLE;
	op->keyt = CK_INVALID_HANDLE;
	op->fg = fg;

	if (op->out == NULL)
		op->outlen = op->inlen;
	return (op);
}

int
cryptotest_close_session(CK_SESSION_HANDLE hsession)
{
	CK_RV rv;
	rv = C_CloseSession(hsession);
	if (rv != CKR_OK)
		cryptotest_error("cryptotest_close_session", rv);

	return (rv);
}

void
cryptotest_close(crypto_op_t *op)
{
	if (op->keyt != CK_INVALID_HANDLE)
		(void) C_DestroyObject(op->hsession, op->keyt);

	if (op->hsession != CK_INVALID_HANDLE)
		(void) cryptotest_close_session(op->hsession);
	free(op);
	VERIFY0(C_Finalize(NULL));
}

int
get_mech_info(crypto_op_t *op)
{
	CK_RV rv;
	rv = pkcs11_str2mech(op->mechname, &op->mech);
	if (rv != CKR_OK) {
		cryptotest_error("get_mech_info", rv);
		(void) fprintf(stderr, "failed to resolve mechanism name %s\n",
		    op->mechname);
		return (CTEST_NAME_RESOLVE_FAILED);
	}
	return (rv);
}


int
get_hsession_by_mech(crypto_op_t *op)
{
	CK_RV rv;
	rv = SUNW_C_GetMechSession(op->mech, &op->hsession);
	if (rv != CKR_OK) {
		cryptotest_error("get_hsession_by_mech", rv);
		(void) fprintf(stderr,
		    "could not find provider for mechanism %lu\n",
		    op->mech);
		return (CTEST_MECH_NO_PROVIDER);
	}
	return (rv);
}

/*
 * SIGN_* functions
 */
int
sign_init(crypto_op_t *op)
{
	CK_MECHANISM mech;
	CK_RV rv;

	mech.mechanism = op->mech;
	mech.pParameter = NULL;
	mech.ulParameterLen = 0;

	rv = SUNW_C_KeyToObject(op->hsession, op->mech,
	    op->key, op->keylen, &op->keyt);

	if (rv != CKR_OK)
		cryptotest_error("SUNW_C_KeyToObject", rv);

	rv = C_SignInit(op->hsession, &mech, op->keyt);

	if (rv != CKR_OK)
		cryptotest_error("C_SignInit", rv);

	return (rv);
}

int
sign_single(crypto_op_t *op)
{
	CK_RV rv;

	rv = C_Sign(op->hsession, op->in, op->inlen,
	    op->out, (CK_ULONG_PTR)&op->outlen);
	if (rv != CKR_OK)
		cryptotest_error("C_Sign", rv);
	return (rv);
}

int
sign_update(crypto_op_t *op, size_t offset, size_t len)
{
	CK_RV rv;
	rv = C_SignUpdate(op->hsession, op->in + offset, len);
	if (rv != CKR_OK)
		cryptotest_error("C_SignUpdate", rv);

	return (rv);
}

int
sign_final(crypto_op_t *op)
{
	CK_RV rv;
	rv = C_SignFinal(op->hsession, op->out, (CK_ULONG_PTR)&op->outlen);
	if (rv != CKR_OK)
		cryptotest_error("C_SignFinal", rv);
	return (rv);
}

/*
 * MAC_* functions
 */
int
mac_init(crypto_op_t *op)
{
	return (sign_init(op));
}

int
mac_single(crypto_op_t *op)
{
	return (sign_single(op));
}

int
mac_update(crypto_op_t *op, size_t offset, size_t len, size_t *dummy __unused)
{
	return (sign_update(op, offset, len));
}

int
mac_final(crypto_op_t *op, size_t dummy __unused)
{
	return (sign_final(op));
}

/*
 * VERIFY_* functions
 */
int
verify_init(crypto_op_t *op)
{
	CK_MECHANISM mech;
	CK_RV rv;

	mech.mechanism = op->mech;
	mech.pParameter = NULL;
	mech.ulParameterLen = 0;

	rv = SUNW_C_KeyToObject(op->hsession, op->mech,
	    op->key, op->keylen, &op->keyt);

	if (rv != CKR_OK)
		cryptotest_error("SUNW_C_KeyToObject", rv);

	rv = C_VerifyInit(op->hsession, &mech, op->keyt);

	if (rv != CKR_OK)
		cryptotest_error("C_VerifyInit", rv);

	return (rv);
}

int
verify_single(crypto_op_t *op)
{
	CK_RV rv;

	rv = C_Verify(op->hsession, op->in, op->inlen, op->out, op->outlen);
	if (rv != CKR_OK && rv != CKR_SIGNATURE_INVALID &&
	    rv != CKR_SIGNATURE_LEN_RANGE)
		cryptotest_error("C_Verify", rv);
	return (rv);
}

int
verify_update(crypto_op_t *op, size_t offset, size_t len)
{
	CK_RV rv;
	rv = C_VerifyUpdate(op->hsession, op->in + offset, len);
	if (rv != CKR_OK)
		cryptotest_error("C_VerifyUpdate", rv);
	return (rv);
}

int
verify_final(crypto_op_t *op)
{
	CK_RV rv;
	rv = C_VerifyFinal(op->hsession, op->out, op->outlen);
	if (rv != CKR_OK && rv != CKR_SIGNATURE_INVALID &&
	    rv != CKR_SIGNATURE_LEN_RANGE)
		cryptotest_error("C_VerifyFinal", rv);
	return (rv);
}

/*
 * ENCRYPT_* functions
 */
int
encrypt_init(crypto_op_t *op)
{
	CK_MECHANISM mech;
	CK_RV rv;

	mech.mechanism = op->mech;
	mech.pParameter = op->param;
	mech.ulParameterLen = op->paramlen;

	rv = SUNW_C_KeyToObject(op->hsession, op->mech,
	    op->key, op->keylen, &op->keyt);

	if (rv != CKR_OK)
		cryptotest_error("SUNW_C_KeyToObject", rv);

	rv = C_EncryptInit(op->hsession, &mech, op->keyt);

	if (rv != CKR_OK)
		cryptotest_error("C_EncryptInit", rv);

	return (rv);
}

int
encrypt_single(crypto_op_t *op)
{
	CK_RV rv;

	rv = C_Encrypt(op->hsession, op->in, op->inlen,
	    op->out, (CK_ULONG_PTR)&op->outlen);
	if (rv != CKR_OK)
		cryptotest_error("C_Encrypt", rv);
	return (rv);
}

int
encrypt_update(crypto_op_t *op, size_t offset, size_t plainlen, size_t *encrlen)
{
	CK_RV rv;
	CK_ULONG outlen = op->outlen - *encrlen;
	rv = C_EncryptUpdate(op->hsession, op->in + offset, plainlen,
	    op->out + *encrlen, &outlen);
	if (rv != CKR_OK)
		cryptotest_error("C_EncryptUpdate", rv);

	*encrlen += outlen;
	return (rv);
}

int
encrypt_final(crypto_op_t *op, size_t encrlen)
{
	CK_RV rv;
	CK_ULONG outlen = op->outlen - encrlen;
	rv = C_EncryptFinal(op->hsession, op->out + encrlen, &outlen);
	if (rv != CKR_OK)
		cryptotest_error("C_EncryptFinal", rv);
	return (rv);
}

/*
 * DECRYPT_* functions
 */
int
decrypt_init(crypto_op_t *op)
{
	CK_MECHANISM mech;
	CK_RV rv;

	mech.mechanism = op->mech;
	mech.pParameter = op->param;
	mech.ulParameterLen = op->paramlen;

	rv = SUNW_C_KeyToObject(op->hsession, op->mech,
	    op->key, op->keylen, &op->keyt);

	if (rv != CKR_OK)
		cryptotest_error("SUNW_C_KeyToObject", rv);

	rv = C_DecryptInit(op->hsession, &mech, op->keyt);

	if (rv != CKR_OK)
		cryptotest_error("C_DecryptInit", rv);

	return (rv);
}

int
decrypt_single(crypto_op_t *op)
{
	CK_RV rv;

	rv = C_Decrypt(op->hsession, op->in, op->inlen,
	    op->out, (CK_ULONG_PTR)&op->outlen);
	if (rv != CKR_OK)
		cryptotest_error("C_Decrypt", rv);
	return (rv);
}

int
decrypt_update(crypto_op_t *op, size_t offset, size_t len, size_t *encrlen)
{
	CK_RV rv;
	CK_ULONG outlen = op->outlen - *encrlen;
	rv = C_DecryptUpdate(op->hsession, op->in + offset, len,
	    op->out + *encrlen, &outlen);
	if (rv != CKR_OK)
		cryptotest_error("C_DecryptUpdate", rv);

	*encrlen += outlen;
	return (rv);
}

int
decrypt_final(crypto_op_t *op, size_t encrlen)
{
	CK_RV rv;
	CK_ULONG outlen = op->outlen - encrlen;
	rv = C_DecryptFinal(op->hsession, op->out + encrlen, &outlen);
	if (rv != CKR_OK)
		cryptotest_error("C_DecryptFinal", rv);
	return (rv);
}

/*
 * DIGEST_ functions
 */
int
digest_init(crypto_op_t *op)
{
	CK_MECHANISM mech;
	CK_RV rv;

	mech.mechanism = op->mech;
	mech.pParameter = op->param;
	mech.ulParameterLen = op->paramlen;

	rv = C_DigestInit(op->hsession, &mech);
	if (rv != CKR_OK)
		cryptotest_error("C_DigestInit", rv);
	return (rv);
}

int
digest_single(crypto_op_t *op)
{
	CK_RV rv;

	rv = C_Digest(op->hsession, op->in, op->inlen,
	    op->out, (CK_ULONG_PTR)&op->outlen);
	if (rv != CKR_OK)
		cryptotest_error("C_Digest", rv);
	return (rv);
}

int
digest_update(crypto_op_t *op, size_t offset, size_t len,
    size_t *dummy __unused)
{
	CK_RV rv;

	rv = C_DigestUpdate(op->hsession, op->in + offset, len);
	if (rv != CKR_OK)
		cryptotest_error("C_DigestUpdate", rv);
	return (rv);
}

int
digest_final(crypto_op_t *op, size_t dummy __unused)
{
	CK_RV rv;

	rv = C_DigestFinal(op->hsession, op->out, (CK_ULONG_PTR)&op->outlen);
	if (rv != CKR_OK)
		cryptotest_error("C_DigestFinal", rv);
	return (rv);
}

void
ccm_init_params(void *buf, ulong_t ulDataLen, uchar_t *pNonce,
    ulong_t ulNonceLen, uchar_t *pAAD, ulong_t ulAADLen, ulong_t ulMACLen)
{
	CK_CCM_PARAMS *pp = buf;

	pp->ulDataLen = ulDataLen;
	pp->pNonce = pNonce;
	pp->ulNonceLen = ulNonceLen;
	pp->pAAD = pAAD;
	pp->ulAADLen = ulAADLen;
	pp->ulMACLen = ulMACLen;
}

size_t
ccm_param_len(void)
{
	return (sizeof (CK_CCM_PARAMS));
}

const char *
cryptotest_errstr(int e, char *buf, size_t buflen)
{
	char *valstr = NULL;

	valstr = pkcs11_strerror(e);

	/*
	 * We'd like both the symbolic and numeric value for every error
	 * value.  pkcs11_strerror() already includes the numeric value
	 * for unknown error values (but not for known values), so we take
	 * advantage of all known PKCS#11 error values starting with 'CKR_'
	 * to determine if we need to include the numeric value or not.
	 */
	if (strcmp(valstr, "CKR_") == 0) {
		(void) snprintf(buf, buflen, "%s (%08x)", valstr, e);
	} else {
		(void) strlcpy(buf, valstr, buflen);
	}

	return (buf);
}
