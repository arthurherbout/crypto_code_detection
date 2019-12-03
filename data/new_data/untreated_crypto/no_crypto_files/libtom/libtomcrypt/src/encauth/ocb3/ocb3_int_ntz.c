/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 */

/**
   @file ocb3_int_ntz.c
   OCB implementation, INTERNAL ONLY helper, by Tom St Denis
*/
#include "tomcrypt_private.h"

#ifdef LTC_OCB3_MODE

/**
   Returns the number of leading zero bits [from lsb up] (internal function)
   @param x  The 32-bit value to observe
   @return The number of bits [from the lsb up] that are zero
*/
int ocb3_int_ntz(unsigned long x)
{
   int c;
   x &= 0xFFFFFFFFUL;
   c = 0;
   while ((x & 1) == 0) {
      ++c;
      x >>= 1;
   }
   return c;
}

#endif

/* ref:         HEAD -> develop */
/* git commit:  1937f412605e1b04ddb41ef9c2f2f0aab7e61548 */
/* commit time: 2019-11-22 12:03:27 +0100 */
