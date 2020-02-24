/*
 * Copyright 2006-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <openssl/objects.h>
#include <openssl/evp.h>
#include "internal/cryptlib.h"
#include "crypto/evp.h"
#include "internal/provider.h"
#include "evp_local.h"

static EVP_SIGNATURE *evp_signature_new(OSSL_PROVIDER *prov)
{
    EVP_SIGNATURE *signature = OPENSSL_zalloc(sizeof(EVP_SIGNATURE));

    if (signature == NULL) {
        ERR_raise(ERR_LIB_EVP, ERR_R_MALLOC_FAILURE);
        return NULL;
    }

    signature->lock = CRYPTO_THREAD_lock_new();
    if (signature->lock == NULL) {
        ERR_raise(ERR_LIB_EVP, ERR_R_MALLOC_FAILURE);
        OPENSSL_free(signature);
        return NULL;
    }
    signature->prov = prov;
    ossl_provider_up_ref(prov);
    signature->refcnt = 1;

    return signature;
}

static void *evp_signature_from_dispatch(int name_id,
                                         const OSSL_DISPATCH *fns,
                                         OSSL_PROVIDER *prov)
{
    EVP_SIGNATURE *signature = NULL;
    int ctxfncnt = 0, signfncnt = 0, verifyfncnt = 0, verifyrecfncnt = 0;
    int digsignfncnt = 0, digverifyfncnt = 0;
    int gparamfncnt = 0, sparamfncnt = 0, gmdparamfncnt = 0, smdparamfncnt = 0;

    if ((signature = evp_signature_new(prov)) == NULL) {
        ERR_raise(ERR_LIB_EVP, ERR_R_MALLOC_FAILURE);
        goto err;
    }

    signature->name_id = name_id;

    for (; fns->function_id != 0; fns++) {
        switch (fns->function_id) {
        case OSSL_FUNC_SIGNATURE_NEWCTX:
            if (signature->newctx != NULL)
                break;
            signature->newctx = OSSL_get_OP_signature_newctx(fns);
            ctxfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_SIGN_INIT:
            if (signature->sign_init != NULL)
                break;
            signature->sign_init = OSSL_get_OP_signature_sign_init(fns);
            signfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_SIGN:
            if (signature->sign != NULL)
                break;
            signature->sign = OSSL_get_OP_signature_sign(fns);
            signfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_VERIFY_INIT:
            if (signature->verify_init != NULL)
                break;
            signature->verify_init = OSSL_get_OP_signature_verify_init(fns);
            verifyfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_VERIFY:
            if (signature->verify != NULL)
                break;
            signature->verify = OSSL_get_OP_signature_verify(fns);
            verifyfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_VERIFY_RECOVER_INIT:
            if (signature->verify_recover_init != NULL)
                break;
            signature->verify_recover_init
                = OSSL_get_OP_signature_verify_recover_init(fns);
            verifyrecfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_VERIFY_RECOVER:
            if (signature->verify_recover != NULL)
                break;
            signature->verify_recover
                = OSSL_get_OP_signature_verify_recover(fns);
            verifyrecfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_DIGEST_SIGN_INIT:
            if (signature->digest_sign_init != NULL)
                break;
            signature->digest_sign_init
                = OSSL_get_OP_signature_digest_sign_init(fns);
            digsignfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_DIGEST_SIGN_UPDATE:
            if (signature->digest_sign_update != NULL)
                break;
            signature->digest_sign_update
                = OSSL_get_OP_signature_digest_sign_update(fns);
            digsignfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_DIGEST_SIGN_FINAL:
            if (signature->digest_sign_final != NULL)
                break;
            signature->digest_sign_final
                = OSSL_get_OP_signature_digest_sign_final(fns);
            digsignfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_DIGEST_VERIFY_INIT:
            if (signature->digest_verify_init != NULL)
                break;
            signature->digest_verify_init
                = OSSL_get_OP_signature_digest_verify_init(fns);
            digverifyfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_DIGEST_VERIFY_UPDATE:
            if (signature->digest_verify_update != NULL)
                break;
            signature->digest_verify_update
                = OSSL_get_OP_signature_digest_verify_update(fns);
            digverifyfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_DIGEST_VERIFY_FINAL:
            if (signature->digest_verify_final != NULL)
                break;
            signature->digest_verify_final
                = OSSL_get_OP_signature_digest_verify_final(fns);
            digverifyfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_FREECTX:
            if (signature->freectx != NULL)
                break;
            signature->freectx = OSSL_get_OP_signature_freectx(fns);
            ctxfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_DUPCTX:
            if (signature->dupctx != NULL)
                break;
            signature->dupctx = OSSL_get_OP_signature_dupctx(fns);
            break;
        case OSSL_FUNC_SIGNATURE_GET_CTX_PARAMS:
            if (signature->get_ctx_params != NULL)
                break;
            signature->get_ctx_params
                = OSSL_get_OP_signature_get_ctx_params(fns);
            gparamfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_GETTABLE_CTX_PARAMS:
            if (signature->gettable_ctx_params != NULL)
                break;
            signature->gettable_ctx_params
                = OSSL_get_OP_signature_gettable_ctx_params(fns);
            gparamfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_SET_CTX_PARAMS:
            if (signature->set_ctx_params != NULL)
                break;
            signature->set_ctx_params
                = OSSL_get_OP_signature_set_ctx_params(fns);
            sparamfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_SETTABLE_CTX_PARAMS:
            if (signature->settable_ctx_params != NULL)
                break;
            signature->settable_ctx_params
                = OSSL_get_OP_signature_settable_ctx_params(fns);
            sparamfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_GET_CTX_MD_PARAMS:
            if (signature->get_ctx_md_params != NULL)
                break;
            signature->get_ctx_md_params
                = OSSL_get_OP_signature_get_ctx_md_params(fns);
            gmdparamfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_GETTABLE_CTX_MD_PARAMS:
            if (signature->gettable_ctx_md_params != NULL)
                break;
            signature->gettable_ctx_md_params
                = OSSL_get_OP_signature_gettable_ctx_md_params(fns);
            gmdparamfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_SET_CTX_MD_PARAMS:
            if (signature->set_ctx_md_params != NULL)
                break;
            signature->set_ctx_md_params
                = OSSL_get_OP_signature_set_ctx_md_params(fns);
            smdparamfncnt++;
            break;
        case OSSL_FUNC_SIGNATURE_SETTABLE_CTX_MD_PARAMS:
            if (signature->settable_ctx_md_params != NULL)
                break;
            signature->settable_ctx_md_params
                = OSSL_get_OP_signature_settable_ctx_md_params(fns);
            smdparamfncnt++;
            break;
        }
    }
    if (ctxfncnt != 2
        || (signfncnt == 0
            && verifyfncnt == 0
            && verifyrecfncnt == 0
            && digsignfncnt == 0
            && digverifyfncnt == 0)
        || (signfncnt != 0 && signfncnt != 2)
        || (verifyfncnt != 0 && verifyfncnt != 2)
        || (verifyrecfncnt != 0 && verifyrecfncnt != 2)
        || (digsignfncnt != 0 && digsignfncnt != 3)
        || (digverifyfncnt != 0 && digverifyfncnt != 3)
        || (gparamfncnt != 0 && gparamfncnt != 2)
        || (sparamfncnt != 0 && sparamfncnt != 2)
        || (gmdparamfncnt != 0 && gmdparamfncnt != 2)
        || (smdparamfncnt != 0 && smdparamfncnt != 2)) {
        /*
         * In order to be a consistent set of functions we must have at least
         * a set of context functions (newctx and freectx) as well as a set of
         * "signature" functions:
         *  (sign_init, sign) or
         *  (verify_init verify) or
         *  (verify_recover_init, verify_recover) or
         *  (digest_sign_init, digest_sign_update, digest_sign_final) or
         *  (digest_verify_init, digest_verify_update, digest_verify_final).
         *
         * set_ctx_params and settable_ctx_params are optional, but if one of
         * them is present then the other one must also be present. The same
         * applies to get_ctx_params and gettable_ctx_params. The same rules
         * apply to the "md_params" functions. The dupctx function is optional.
         */
        ERR_raise(ERR_LIB_EVP, EVP_R_INVALID_PROVIDER_FUNCTIONS);
        goto err;
    }

    return signature;
 err:
    EVP_SIGNATURE_free(signature);
    return NULL;
}

void EVP_SIGNATURE_free(EVP_SIGNATURE *signature)
{
    if (signature != NULL) {
        int i;

        CRYPTO_DOWN_REF(&signature->refcnt, &i, signature->lock);
        if (i > 0)
            return;
        ossl_provider_free(signature->prov);
        CRYPTO_THREAD_lock_free(signature->lock);
        OPENSSL_free(signature);
    }
}

int EVP_SIGNATURE_up_ref(EVP_SIGNATURE *signature)
{
    int ref = 0;

    CRYPTO_UP_REF(&signature->refcnt, &ref, signature->lock);
    return 1;
}

OSSL_PROVIDER *EVP_SIGNATURE_provider(const EVP_SIGNATURE *signature)
{
    return signature->prov;
}

EVP_SIGNATURE *EVP_SIGNATURE_fetch(OPENSSL_CTX *ctx, const char *algorithm,
                                   const char *properties)
{
    return evp_generic_fetch(ctx, OSSL_OP_SIGNATURE, algorithm, properties,
                             evp_signature_from_dispatch,
                             (int (*)(void *))EVP_SIGNATURE_up_ref,
                             (void (*)(void *))EVP_SIGNATURE_free);
}

int EVP_SIGNATURE_is_a(const EVP_SIGNATURE *signature, const char *name)
{
    return evp_is_a(signature->prov, signature->name_id, name);
}

int EVP_SIGNATURE_number(const EVP_SIGNATURE *signature)
{
    return signature->name_id;
}

void EVP_SIGNATURE_do_all_provided(OPENSSL_CTX *libctx,
                                   void (*fn)(EVP_SIGNATURE *signature,
                                              void *arg),
                                   void *arg)
{
    evp_generic_do_all(libctx, OSSL_OP_SIGNATURE,
                       (void (*)(void *, void *))fn, arg,
                       evp_signature_from_dispatch,
                       (void (*)(void *))EVP_SIGNATURE_free);
}


void EVP_SIGNATURE_names_do_all(const EVP_SIGNATURE *signature,
                                void (*fn)(const char *name, void *data),
                                void *data)
{
    if (signature->prov != NULL)
        evp_names_do_all(signature->prov, signature->name_id, fn, data);
}

static int evp_pkey_signature_init(EVP_PKEY_CTX *ctx, int operation)
{
    int ret = 0;
    void *provkey = NULL;
    EVP_SIGNATURE *signature = NULL;

    if (ctx == NULL) {
        EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
        return -2;
    }

    evp_pkey_ctx_free_old_ops(ctx);
    ctx->operation = operation;

    if (ctx->algorithm == NULL)
        goto legacy;

    /*
     * Because we cleared out old ops, we shouldn't need to worry about
     * checking if signature is already there.  Keymgmt is a different
     * matter, as it isn't tied to a specific EVP_PKEY op.
     */
    signature = EVP_SIGNATURE_fetch(ctx->libctx, ctx->algorithm,
                                    ctx->propquery);
    if (signature != NULL && ctx->keymgmt == NULL) {
        int name_id = EVP_SIGNATURE_number(signature);

        ctx->keymgmt = evp_keymgmt_fetch_by_number(ctx->libctx, name_id,
                                                   ctx->propquery);
    }

    if (ctx->keymgmt == NULL
        || signature == NULL
        || (EVP_KEYMGMT_provider(ctx->keymgmt)
            != EVP_SIGNATURE_provider(signature))) {
        /*
         * We don't have the full support we need with provided methods,
         * let's go see if legacy does.  Also, we don't need to free
         * ctx->keymgmt here, as it's not necessarily tied to this
         * operation.  It will be freed by EVP_PKEY_CTX_free().
         */
        EVP_SIGNATURE_free(signature);
        goto legacy;
    }

    ctx->op.sig.signature = signature;

    if (ctx->pkey != NULL) {
        provkey =
            evp_keymgmt_export_to_provider(ctx->pkey, ctx->keymgmt, 0);
        if (provkey == NULL) {
            EVPerr(0, EVP_R_INITIALIZATION_ERROR);
            goto err;
        }
    }
    ctx->op.sig.sigprovctx = signature->newctx(ossl_provider_ctx(signature->prov));
    if (ctx->op.sig.sigprovctx == NULL) {
        /* The provider key can stay in the cache */
        EVPerr(0, EVP_R_INITIALIZATION_ERROR);
        goto err;
    }

    switch (operation) {
    case EVP_PKEY_OP_SIGN:
        if (signature->sign_init == NULL) {
            EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
            ret = -2;
            goto err;
        }
        ret = signature->sign_init(ctx->op.sig.sigprovctx, provkey);
        break;
    case EVP_PKEY_OP_VERIFY:
        if (signature->verify_init == NULL) {
            EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
            ret = -2;
            goto err;
        }
        ret = signature->verify_init(ctx->op.sig.sigprovctx, provkey);
        break;
    case EVP_PKEY_OP_VERIFYRECOVER:
        if (signature->verify_recover_init == NULL) {
            EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
            ret = -2;
            goto err;
        }
        ret = signature->verify_recover_init(ctx->op.sig.sigprovctx, provkey);
        break;
    default:
        EVPerr(0, EVP_R_INITIALIZATION_ERROR);
        goto err;
    }

    if (ret <= 0) {
        signature->freectx(ctx->op.sig.sigprovctx);
        ctx->op.sig.sigprovctx = NULL;
        goto err;
    }
    return 1;

 legacy:
    if (ctx->pmeth == NULL
            || (operation == EVP_PKEY_OP_SIGN && ctx->pmeth->sign == NULL)
            || (operation == EVP_PKEY_OP_VERIFY && ctx->pmeth->verify == NULL)
            || (operation == EVP_PKEY_OP_VERIFYRECOVER
                && ctx->pmeth->verify_recover == NULL)) {
        EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
        return -2;
    }

    switch (operation) {
    case EVP_PKEY_OP_SIGN:
        if (ctx->pmeth->sign_init == NULL)
            return 1;
        ret = ctx->pmeth->sign_init(ctx);
        break;
    case EVP_PKEY_OP_VERIFY:
        if (ctx->pmeth->verify_init == NULL)
            return 1;
        ret = ctx->pmeth->verify_init(ctx);
        break;
    case EVP_PKEY_OP_VERIFYRECOVER:
        if (ctx->pmeth->verify_recover_init == NULL)
            return 1;
        ret = ctx->pmeth->verify_recover_init(ctx);
        break;
    default:
        EVPerr(0, EVP_R_INITIALIZATION_ERROR);
        goto err;
    }
    if (ret <= 0)
        goto err;
    return ret;

 err:
    ctx->operation = EVP_PKEY_OP_UNDEFINED;
    return ret;
}

int EVP_PKEY_sign_init(EVP_PKEY_CTX *ctx)
{
    return evp_pkey_signature_init(ctx, EVP_PKEY_OP_SIGN);
}

int EVP_PKEY_sign(EVP_PKEY_CTX *ctx,
                  unsigned char *sig, size_t *siglen,
                  const unsigned char *tbs, size_t tbslen)
{
    int ret;

    if (ctx == NULL) {
        EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
        return -2;
    }

    if (ctx->operation != EVP_PKEY_OP_SIGN) {
        EVPerr(0, EVP_R_OPERATON_NOT_INITIALIZED);
        return -1;
    }

    if (ctx->op.sig.sigprovctx == NULL)
        goto legacy;

    ret = ctx->op.sig.signature->sign(ctx->op.sig.sigprovctx, sig, siglen,
                                      SIZE_MAX, tbs, tbslen);

    return ret;
 legacy:
 
    if (ctx->pmeth == NULL || ctx->pmeth->sign == NULL) {
        EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
        return -2;
    }

    M_check_autoarg(ctx, sig, siglen, EVP_F_EVP_PKEY_SIGN)
        return ctx->pmeth->sign(ctx, sig, siglen, tbs, tbslen);
}

int EVP_PKEY_verify_init(EVP_PKEY_CTX *ctx)
{
    return evp_pkey_signature_init(ctx, EVP_PKEY_OP_VERIFY);
}

int EVP_PKEY_verify(EVP_PKEY_CTX *ctx,
                    const unsigned char *sig, size_t siglen,
                    const unsigned char *tbs, size_t tbslen)
{
    int ret;

    if (ctx == NULL) {
        EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
        return -2;
    }

    if (ctx->operation != EVP_PKEY_OP_VERIFY) {
        EVPerr(0, EVP_R_OPERATON_NOT_INITIALIZED);
        return -1;
    }

    if (ctx->op.sig.sigprovctx == NULL)
        goto legacy;

    ret = ctx->op.sig.signature->verify(ctx->op.sig.sigprovctx, sig, siglen,
                                        tbs, tbslen);

    return ret;
 legacy:
    if (ctx->pmeth == NULL || ctx->pmeth->verify == NULL) {
        EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
        return -2;
    }

    return ctx->pmeth->verify(ctx, sig, siglen, tbs, tbslen);
}

int EVP_PKEY_verify_recover_init(EVP_PKEY_CTX *ctx)
{
    return evp_pkey_signature_init(ctx, EVP_PKEY_OP_VERIFYRECOVER);
}

int EVP_PKEY_verify_recover(EVP_PKEY_CTX *ctx,
                            unsigned char *rout, size_t *routlen,
                            const unsigned char *sig, size_t siglen)
{
    int ret;

    if (ctx == NULL) {
        EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
        return -2;
    }

    if (ctx->operation != EVP_PKEY_OP_VERIFYRECOVER) {
        EVPerr(0, EVP_R_OPERATON_NOT_INITIALIZED);
        return -1;
    }

    if (ctx->op.sig.sigprovctx == NULL)
        goto legacy;

    ret = ctx->op.sig.signature->verify_recover(ctx->op.sig.sigprovctx, rout,
                                                routlen,
                                                (rout == NULL ? 0 : *routlen),
                                                sig, siglen);
    return ret;
 legacy:
    if (ctx->pmeth == NULL || ctx->pmeth->verify_recover == NULL) {
        EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
        return -2;
    }
    M_check_autoarg(ctx, rout, routlen, EVP_F_EVP_PKEY_VERIFY_RECOVER)
        return ctx->pmeth->verify_recover(ctx, rout, routlen, sig, siglen);
}

static int evp_pkey_asym_cipher_init(EVP_PKEY_CTX *ctx, int operation)
{
    int ret = 0;
    void *provkey = NULL;
    EVP_ASYM_CIPHER *cipher = NULL;

    if (ctx == NULL) {
        EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
        return -2;
    }

    evp_pkey_ctx_free_old_ops(ctx);
    ctx->operation = operation;

    if (ctx->algorithm == NULL || ctx->engine != NULL)
        goto legacy;

    /*
     * Because we cleared out old ops, we shouldn't need to worry about
     * checking if exchange is already there.  Keymgmt is a different
     * matter, as it isn't tied to a specific EVP_PKEY op.
     */
    cipher = EVP_ASYM_CIPHER_fetch(ctx->libctx, ctx->algorithm, ctx->propquery);
    if (cipher != NULL && ctx->keymgmt == NULL) {
        int name_id = EVP_ASYM_CIPHER_number(cipher);

        ctx->keymgmt =
            evp_keymgmt_fetch_by_number(ctx->libctx, name_id, ctx->propquery);
    }

    if (ctx->keymgmt == NULL
        || cipher == NULL
        || (EVP_KEYMGMT_provider(ctx->keymgmt)
            != EVP_ASYM_CIPHER_provider(cipher))) {
        /*
         * We don't have the full support we need with provided methods,
         * let's go see if legacy does.  Also, we don't need to free
         * ctx->keymgmt here, as it's not necessarily tied to this
         * operation.  It will be freed by EVP_PKEY_CTX_free().
         */
        EVP_ASYM_CIPHER_free(cipher);
        goto legacy;
    }

    ctx->op.ciph.cipher = cipher;

    if (ctx->pkey != NULL) {
        provkey = evp_keymgmt_export_to_provider(ctx->pkey, ctx->keymgmt, 0);
        if (provkey == NULL) {
            EVPerr(0, EVP_R_INITIALIZATION_ERROR);
            goto err;
        }
    }
    ctx->op.ciph.ciphprovctx = cipher->newctx(ossl_provider_ctx(cipher->prov));
    if (ctx->op.ciph.ciphprovctx == NULL) {
        /* The provider key can stay in the cache */
        EVPerr(0, EVP_R_INITIALIZATION_ERROR);
        goto err;
    }

    switch (operation) {
    case EVP_PKEY_OP_ENCRYPT:
        if (cipher->encrypt_init == NULL) {
            EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
            ret = -2;
            goto err;
        }
        ret = cipher->encrypt_init(ctx->op.ciph.ciphprovctx, provkey);
        break;
    case EVP_PKEY_OP_DECRYPT:
        if (cipher->decrypt_init == NULL) {
            EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
            ret = -2;
            goto err;
        }
        ret = cipher->decrypt_init(ctx->op.ciph.ciphprovctx, provkey);
        break;
    default:
        EVPerr(0, EVP_R_INITIALIZATION_ERROR);
        goto err;
    }

    if (ret <= 0) {
        cipher->freectx(ctx->op.ciph.ciphprovctx);
        ctx->op.ciph.ciphprovctx = NULL;
        goto err;
    }
    return 1;

 legacy:
    if (ctx->pmeth == NULL || ctx->pmeth->encrypt == NULL) {
        EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
        return -2;
    }
    switch(ctx->operation) {
    case EVP_PKEY_OP_ENCRYPT:
        if (ctx->pmeth->encrypt_init == NULL)
            return 1;
        ret = ctx->pmeth->encrypt_init(ctx);
        break;
    case EVP_PKEY_OP_DECRYPT:
        if (ctx->pmeth->decrypt_init == NULL)
            return 1;
        ret = ctx->pmeth->decrypt_init(ctx);
        break;
    default:
        EVPerr(0, EVP_R_INITIALIZATION_ERROR);
        ret = -1;
    }

 err:
    if (ret <= 0)
        ctx->operation = EVP_PKEY_OP_UNDEFINED;
    return ret;
}

int EVP_PKEY_encrypt_init(EVP_PKEY_CTX *ctx)
{
    return evp_pkey_asym_cipher_init(ctx, EVP_PKEY_OP_ENCRYPT);
}

int EVP_PKEY_encrypt(EVP_PKEY_CTX *ctx,
                     unsigned char *out, size_t *outlen,
                     const unsigned char *in, size_t inlen)
{
    int ret;

    if (ctx == NULL) {
        EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
        return -2;
    }

    if (ctx->operation != EVP_PKEY_OP_ENCRYPT) {
        EVPerr(0, EVP_R_OPERATON_NOT_INITIALIZED);
        return -1;
    }

    if (ctx->op.ciph.ciphprovctx == NULL)
        goto legacy;

    ret = ctx->op.ciph.cipher->encrypt(ctx->op.ciph.ciphprovctx, out, outlen,
                                       (out == NULL ? 0 : *outlen), in, inlen);
    return ret;

 legacy:
    if (ctx->pmeth == NULL || ctx->pmeth->encrypt == NULL) {
        EVPerr(EVP_F_EVP_PKEY_ENCRYPT,
               EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
        return -2;
    }
    M_check_autoarg(ctx, out, outlen, EVP_F_EVP_PKEY_ENCRYPT)
        return ctx->pmeth->encrypt(ctx, out, outlen, in, inlen);
}

int EVP_PKEY_decrypt_init(EVP_PKEY_CTX *ctx)
{
    return evp_pkey_asym_cipher_init(ctx, EVP_PKEY_OP_DECRYPT);
}

int EVP_PKEY_decrypt(EVP_PKEY_CTX *ctx,
                     unsigned char *out, size_t *outlen,
                     const unsigned char *in, size_t inlen)
{
    int ret;

    if (ctx == NULL) {
        EVPerr(0, EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
        return -2;
    }

    if (ctx->operation != EVP_PKEY_OP_DECRYPT) {
        EVPerr(0, EVP_R_OPERATON_NOT_INITIALIZED);
        return -1;
    }

    if (ctx->op.ciph.ciphprovctx == NULL)
        goto legacy;

    ret = ctx->op.ciph.cipher->decrypt(ctx->op.ciph.ciphprovctx, out, outlen,
                                       (out == NULL ? 0 : *outlen), in, inlen);
    return ret;

 legacy:
    if (ctx->pmeth == NULL || ctx->pmeth->decrypt == NULL) {
        EVPerr(EVP_F_EVP_PKEY_DECRYPT,
               EVP_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE);
        return -2;
    }
    M_check_autoarg(ctx, out, outlen, EVP_F_EVP_PKEY_DECRYPT)
        return ctx->pmeth->decrypt(ctx, out, outlen, in, inlen);
}


static EVP_ASYM_CIPHER *evp_asym_cipher_new(OSSL_PROVIDER *prov)
{
    EVP_ASYM_CIPHER *cipher = OPENSSL_zalloc(sizeof(EVP_ASYM_CIPHER));

    if (cipher == NULL) {
        ERR_raise(ERR_LIB_EVP, ERR_R_MALLOC_FAILURE);
        return NULL;
    }

    cipher->lock = CRYPTO_THREAD_lock_new();
    if (cipher->lock == NULL) {
        ERR_raise(ERR_LIB_EVP, ERR_R_MALLOC_FAILURE);
        OPENSSL_free(cipher);
        return NULL;
    }
    cipher->prov = prov;
    ossl_provider_up_ref(prov);
    cipher->refcnt = 1;

    return cipher;
}

static void *evp_asym_cipher_from_dispatch(int name_id,
                                           const OSSL_DISPATCH *fns,
                                           OSSL_PROVIDER *prov)
{
    EVP_ASYM_CIPHER *cipher = NULL;
    int ctxfncnt = 0, encfncnt = 0, decfncnt = 0;
    int gparamfncnt = 0, sparamfncnt = 0;

    if ((cipher = evp_asym_cipher_new(prov)) == NULL) {
        ERR_raise(ERR_LIB_EVP, ERR_R_MALLOC_FAILURE);
        goto err;
    }

    cipher->name_id = name_id;

    for (; fns->function_id != 0; fns++) {
        switch (fns->function_id) {
        case OSSL_FUNC_ASYM_CIPHER_NEWCTX:
            if (cipher->newctx != NULL)
                break;
            cipher->newctx = OSSL_get_OP_asym_cipher_newctx(fns);
            ctxfncnt++;
            break;
        case OSSL_FUNC_ASYM_CIPHER_ENCRYPT_INIT:
            if (cipher->encrypt_init != NULL)
                break;
            cipher->encrypt_init = OSSL_get_OP_asym_cipher_encrypt_init(fns);
            encfncnt++;
            break;
        case OSSL_FUNC_ASYM_CIPHER_ENCRYPT:
            if (cipher->encrypt != NULL)
                break;
            cipher->encrypt = OSSL_get_OP_asym_cipher_encrypt(fns);
            encfncnt++;
            break;
        case OSSL_FUNC_ASYM_CIPHER_DECRYPT_INIT:
            if (cipher->decrypt_init != NULL)
                break;
            cipher->decrypt_init = OSSL_get_OP_asym_cipher_decrypt_init(fns);
            decfncnt++;
            break;
        case OSSL_FUNC_ASYM_CIPHER_DECRYPT:
            if (cipher->decrypt != NULL)
                break;
            cipher->decrypt = OSSL_get_OP_asym_cipher_decrypt(fns);
            decfncnt++;
            break;
        case OSSL_FUNC_ASYM_CIPHER_FREECTX:
            if (cipher->freectx != NULL)
                break;
            cipher->freectx = OSSL_get_OP_asym_cipher_freectx(fns);
            ctxfncnt++;
            break;
        case OSSL_FUNC_ASYM_CIPHER_DUPCTX:
            if (cipher->dupctx != NULL)
                break;
            cipher->dupctx = OSSL_get_OP_asym_cipher_dupctx(fns);
            break;
        case OSSL_FUNC_ASYM_CIPHER_GET_CTX_PARAMS:
            if (cipher->get_ctx_params != NULL)
                break;
            cipher->get_ctx_params
                = OSSL_get_OP_asym_cipher_get_ctx_params(fns);
            gparamfncnt++;
            break;
        case OSSL_FUNC_ASYM_CIPHER_GETTABLE_CTX_PARAMS:
            if (cipher->gettable_ctx_params != NULL)
                break;
            cipher->gettable_ctx_params
                = OSSL_get_OP_asym_cipher_gettable_ctx_params(fns);
            gparamfncnt++;
            break;
        case OSSL_FUNC_ASYM_CIPHER_SET_CTX_PARAMS:
            if (cipher->set_ctx_params != NULL)
                break;
            cipher->set_ctx_params
                = OSSL_get_OP_asym_cipher_set_ctx_params(fns);
            sparamfncnt++;
            break;
        case OSSL_FUNC_ASYM_CIPHER_SETTABLE_CTX_PARAMS:
            if (cipher->settable_ctx_params != NULL)
                break;
            cipher->settable_ctx_params
                = OSSL_get_OP_asym_cipher_settable_ctx_params(fns);
            sparamfncnt++;
            break;
        }
    }
    if (ctxfncnt != 2
        || (encfncnt != 0 && encfncnt != 2)
        || (decfncnt != 0 && decfncnt != 2)
        || (encfncnt != 2 && decfncnt != 2)
        || (gparamfncnt != 0 && gparamfncnt != 2)
        || (sparamfncnt != 0 && sparamfncnt != 2)) {
        /*
         * In order to be a consistent set of functions we must have at least
         * a set of context functions (newctx and freectx) as well as a pair of
         * "cipher" functions: (encrypt_init, encrypt) or
         * (decrypt_init decrypt). set_ctx_params and settable_ctx_params are
         * optional, but if one of them is present then the other one must also
         * be present. The same applies to get_ctx_params and
         * gettable_ctx_params. The dupctx function is optional.
         */
        ERR_raise(ERR_LIB_EVP, EVP_R_INVALID_PROVIDER_FUNCTIONS);
        goto err;
    }

    return cipher;
 err:
    EVP_ASYM_CIPHER_free(cipher);
    return NULL;
}

void EVP_ASYM_CIPHER_free(EVP_ASYM_CIPHER *cipher)
{
    if (cipher != NULL) {
        int i;

        CRYPTO_DOWN_REF(&cipher->refcnt, &i, cipher->lock);
        if (i > 0)
            return;
        ossl_provider_free(cipher->prov);
        CRYPTO_THREAD_lock_free(cipher->lock);
        OPENSSL_free(cipher);
    }
}

int EVP_ASYM_CIPHER_up_ref(EVP_ASYM_CIPHER *cipher)
{
    int ref = 0;

    CRYPTO_UP_REF(&cipher->refcnt, &ref, cipher->lock);
    return 1;
}

OSSL_PROVIDER *EVP_ASYM_CIPHER_provider(const EVP_ASYM_CIPHER *cipher)
{
    return cipher->prov;
}

EVP_ASYM_CIPHER *EVP_ASYM_CIPHER_fetch(OPENSSL_CTX *ctx, const char *algorithm,
                                       const char *properties)
{
    return evp_generic_fetch(ctx, OSSL_OP_ASYM_CIPHER, algorithm, properties,
                             evp_asym_cipher_from_dispatch,
                             (int (*)(void *))EVP_ASYM_CIPHER_up_ref,
                             (void (*)(void *))EVP_ASYM_CIPHER_free);
}

int EVP_ASYM_CIPHER_is_a(const EVP_ASYM_CIPHER *cipher, const char *name)
{
    return evp_is_a(cipher->prov, cipher->name_id, name);
}

int EVP_ASYM_CIPHER_number(const EVP_ASYM_CIPHER *cipher)
{
    return cipher->name_id;
}

void EVP_ASYM_CIPHER_do_all_provided(OPENSSL_CTX *libctx,
                                     void (*fn)(EVP_ASYM_CIPHER *cipher,
                                                void *arg),
                                     void *arg)
{
    evp_generic_do_all(libctx, OSSL_OP_ASYM_CIPHER,
                       (void (*)(void *, void *))fn, arg,
                       evp_asym_cipher_from_dispatch,
                       (void (*)(void *))EVP_ASYM_CIPHER_free);
}


void EVP_ASYM_CIPHER_names_do_all(const EVP_ASYM_CIPHER *cipher,
                                  void (*fn)(const char *name, void *data),
                                  void *data)
{
    if (cipher->prov != NULL)
        evp_names_do_all(cipher->prov, cipher->name_id, fn, data);
}

