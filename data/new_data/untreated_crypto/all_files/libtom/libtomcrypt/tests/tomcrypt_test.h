/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 */

#ifndef __TEST_H_
#define __TEST_H_

#include "tomcrypt_private.h"

#include "common.h"

typedef struct {
    char *name, *prov, *req;
    int  (*entry)(void);
} test_entry;

/* TESTS */
int cipher_hash_test(void);
int modes_test(void);
int mac_test(void);
int pkcs_1_test(void);
int pkcs_1_pss_test(void);
int pkcs_1_oaep_test(void);
int pkcs_1_emsa_test(void);
int pkcs_1_eme_test(void);
int store_test(void);
int rotate_test(void);
int rsa_test(void);
int dh_test(void);
int ecc_test(void);
int dsa_test(void);
int der_test(void);
int misc_test(void);
int base64_test(void);
int base32_test(void);
int base16_test(void);
int file_test(void);
int multi_test(void);
int prng_test(void);
int mpi_test(void);
int padding_test(void);
int x25519_test(void);
int ed25519_test(void);
int ssh_test(void);
int bcrypt_test(void);

#ifdef LTC_PKCS_1
struct ltc_prng_descriptor* no_prng_desc_get(void);
void no_prng_desc_free(struct ltc_prng_descriptor*);
#endif

#endif

/* ref:         HEAD -> develop */
/* git commit:  1937f412605e1b04ddb41ef9c2f2f0aab7e61548 */
/* commit time: 2019-11-22 12:03:27 +0100 */
