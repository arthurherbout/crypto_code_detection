/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 */
#include "tomcrypt_private.h"

/**
  @file x25519_import_raw.c
  Set the parameters of a X25519 key, Steffen Jaeckel
*/

#ifdef LTC_CURVE25519

/**
   Set the parameters of a X25519 key

   @param in       The key
   @param inlen    The length of the key
   @param which    Which type of key (PK_PRIVATE or PK_PUBLIC)
   @param key      [out] Destination of the key
   @return CRYPT_OK if successful
*/
int x25519_import_raw(const unsigned char *in, unsigned long inlen, int which, curve25519_key *key)
{
   LTC_ARGCHK(in   != NULL);
   LTC_ARGCHK(inlen == 32uL);
   LTC_ARGCHK(key  != NULL);

   if (which == PK_PRIVATE) {
      XMEMCPY(key->priv, in, sizeof(key->priv));
      tweetnacl_crypto_scalarmult_base(key->pub, key->priv);
   } else if (which == PK_PUBLIC) {
      XMEMCPY(key->pub, in, sizeof(key->pub));
   } else {
      return CRYPT_INVALID_ARG;
   }
   key->algo = PKA_X25519;
   key->type = which;

   return CRYPT_OK;
}

#endif

/* ref:         HEAD -> develop */
/* git commit:  1937f412605e1b04ddb41ef9c2f2f0aab7e61548 */
/* commit time: 2019-11-22 12:03:27 +0100 */
