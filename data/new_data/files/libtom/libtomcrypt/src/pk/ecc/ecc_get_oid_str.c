/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 */

#include "tomcrypt_private.h"

#ifdef LTC_MECC

/** Extract OID as a string from ECC key
  @param out    [out] destination buffer
  @param outlen [in/out] Length of destination buffer and final output size (without terminating NUL byte)
  @param key    The ECC key
  Return        CRYPT_OK on success
*/

int ecc_get_oid_str(char *out, unsigned long *outlen, const ecc_key *key)
{
   LTC_ARGCHK(key != NULL);

   return pk_oid_num_to_str(key->dp.oid, key->dp.oidlen, out, outlen);
}

#endif

/* ref:         HEAD -> develop */
/* git commit:  1937f412605e1b04ddb41ef9c2f2f0aab7e61548 */
/* commit time: 2019-11-22 12:03:27 +0100 */
