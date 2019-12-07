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
  @file crypt_unregister_cipher.c
  Unregister a cipher, Tom St Denis
*/

/**
  Unregister a cipher from the descriptor table
  @param cipher   The cipher descriptor to remove
  @return CRYPT_OK on success
*/
int unregister_cipher(const struct ltc_cipher_descriptor *cipher)
{
   int x;

   LTC_ARGCHK(cipher != NULL);

   /* is it already registered? */
   LTC_MUTEX_LOCK(&ltc_cipher_mutex);
   for (x = 0; x < TAB_SIZE; x++) {
       if (XMEMCMP(&cipher_descriptor[x], cipher, sizeof(struct ltc_cipher_descriptor)) == 0) {
          cipher_descriptor[x].name = NULL;
          cipher_descriptor[x].ID   = 255;
          LTC_MUTEX_UNLOCK(&ltc_cipher_mutex);
          return CRYPT_OK;
       }
   }
   LTC_MUTEX_UNLOCK(&ltc_cipher_mutex);
   return CRYPT_ERROR;
}

/* ref:         HEAD -> develop */
/* git commit:  1937f412605e1b04ddb41ef9c2f2f0aab7e61548 */
/* commit time: 2019-11-22 12:03:27 +0100 */
