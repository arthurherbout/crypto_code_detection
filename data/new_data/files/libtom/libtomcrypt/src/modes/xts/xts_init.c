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
  Source donated by Elliptic Semiconductor Inc (www.ellipticsemi.com) to the LibTom Projects
*/

#ifdef LTC_XTS_MODE

/** Start XTS mode
   @param cipher      The index of the cipher to use
   @param key1        The encrypt key
   @param key2        The tweak encrypt key
   @param keylen      The length of the keys (each) in octets
   @param num_rounds  The number of rounds for the cipher (0 == default)
   @param xts         [out] XTS structure
   Returns CRYPT_OK upon success.
*/
int xts_start(int cipher, const unsigned char *key1, const unsigned char *key2, unsigned long keylen, int num_rounds,
              symmetric_xts *xts)
{
   int err;

   /* check inputs */
   LTC_ARGCHK(key1 != NULL);
   LTC_ARGCHK(key2 != NULL);
   LTC_ARGCHK(xts != NULL);

   /* check if valid */
   if ((err = cipher_is_valid(cipher)) != CRYPT_OK) {
      return err;
   }

   if (cipher_descriptor[cipher].block_length != 16) {
      return CRYPT_INVALID_ARG;
   }

   /* schedule the two ciphers */
   if ((err = cipher_descriptor[cipher].setup(key1, keylen, num_rounds, &xts->key1)) != CRYPT_OK) {
      return err;
   }
   if ((err = cipher_descriptor[cipher].setup(key2, keylen, num_rounds, &xts->key2)) != CRYPT_OK) {
      return err;
   }
   xts->cipher = cipher;

   return err;
}

#endif

/* ref:         HEAD -> develop */
/* git commit:  1937f412605e1b04ddb41ef9c2f2f0aab7e61548 */
/* commit time: 2019-11-22 12:03:27 +0100 */
