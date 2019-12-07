/*
 * Copyright 2015-2019 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <openssl/opensslconf.h>

#ifndef OPENSSL_NO_MD2

# include <openssl/md2.h>
# include "crypto/evp.h"

static const EVP_MD md2_md = {
    NID_md2,
    NID_md2WithRSAEncryption,
    MD2_DIGEST_LENGTH,
    0,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    MD2_BLOCK,
};

const EVP_MD *EVP_md2(void)
{
    return &md2_md;
}

#endif /* OPENSSL_NO_MD2 */
