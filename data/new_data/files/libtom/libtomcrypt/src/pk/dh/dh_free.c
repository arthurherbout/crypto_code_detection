/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 */

#include "tomcrypt_private.h"

#ifdef LTC_MDH

/**
  Free the allocated ram for a DH key
  @param key   The key which you wish to free
*/
void dh_free(dh_key *key)
{
   LTC_ARGCHKVD(key != NULL);
   mp_cleanup_multi(&key->prime, &key->base, &key->y, &key->x, NULL);
}

#endif /* LTC_MDH */

/* ref:         HEAD -> develop */
/* git commit:  1937f412605e1b04ddb41ef9c2f2f0aab7e61548 */
/* commit time: 2019-11-22 12:03:27 +0100 */
