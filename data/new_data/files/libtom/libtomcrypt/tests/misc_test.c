/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 */
#include  <tomcrypt_test.h>

int misc_test(void)
{
#ifdef LTC_BCRYPT
   DO(bcrypt_test());
#endif
#ifdef LTC_HKDF
   DO(hkdf_test());
#endif
#ifdef LTC_PKCS_5
   DO(pkcs_5_test());
#endif
#ifdef LTC_PADDING
   DO(padding_test());
#endif
#ifdef LTC_BASE64
   DO(base64_test());
#endif
#ifdef LTC_BASE32
   DO(base32_test());
#endif
#ifdef LTC_BASE16
   DO(base16_test());
#endif
#ifdef LTC_ADLER32
   DO(adler32_test());
#endif
#ifdef LTC_CRC32
   DO(crc32_test());
#endif
#ifdef LTC_SSH
   ssh_test();
#endif
   return 0;
}

/* ref:         HEAD -> develop */
/* git commit:  1937f412605e1b04ddb41ef9c2f2f0aab7e61548 */
/* commit time: 2019-11-22 12:03:27 +0100 */
