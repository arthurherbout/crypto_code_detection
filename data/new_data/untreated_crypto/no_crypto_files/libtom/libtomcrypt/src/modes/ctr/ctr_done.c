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
   @file ctr_done.c
   CTR implementation, finish chain, Tom St Denis
*/

#ifdef LTC_CTR_MODE

/** Terminate the chain
  @param ctr    The CTR chain to terminate
  @return CRYPT_OK on success
*/
int ctr_done(symmetric_CTR *ctr)
{
   int err;
   LTC_ARGCHK(ctr != NULL);

   if ((err = cipher_is_valid(ctr->cipher)) != CRYPT_OK) {
      return err;
   }
   cipher_descriptor[ctr->cipher].done(&ctr->key);
   return CRYPT_OK;
}



#endif

/* ref:         HEAD -> develop */
/* git commit:  1937f412605e1b04ddb41ef9c2f2f0aab7e61548 */
/* commit time: 2019-11-22 12:03:27 +0100 */
