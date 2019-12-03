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
   @file lrw_test.c
   LRW_MODE implementation, test LRW, Tom St Denis
*/

#ifdef LTC_LRW_MODE

/**
  Test LRW against specs
  @return CRYPT_OK if goodly
*/
int lrw_test(void)
{
#ifndef  LTC_TEST
   return CRYPT_NOP;
#else
   static const struct {
      unsigned char key[16], tweak[16], IV[16], P[16], expected_tweak[16], C[16];
   } tests[] = {

{
{ 0x45, 0x62, 0xac, 0x25, 0xf8, 0x28, 0x17, 0x6d, 0x4c, 0x26, 0x84, 0x14, 0xb5, 0x68, 0x01, 0x85 },
{ 0x25, 0x8e, 0x2a, 0x05, 0xe7, 0x3e, 0x9d, 0x03, 0xee, 0x5a, 0x83, 0x0c, 0xcc, 0x09, 0x4c, 0x87 },
{ 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
{ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46 },
{ 0x25, 0x8e, 0x2a, 0x05, 0xe7, 0x3e, 0x9d, 0x03, 0xee, 0x5a, 0x83, 0x0c, 0xcc, 0x09, 0x4c, 0x87 },
{ 0xf1, 0xb2, 0x73, 0xcd, 0x65, 0xa3, 0xdf, 0x5f, 0xe9, 0x5d, 0x48, 0x92, 0x54, 0x63, 0x4e, 0xb8 }
},

{
{ 0x59, 0x70, 0x47, 0x14, 0xf5, 0x57, 0x47, 0x8c, 0xd7, 0x79, 0xe8, 0x0f, 0x54, 0x88, 0x79, 0x44 },
{ 0x35, 0x23, 0xc2, 0xde, 0xc5, 0x69, 0x4f, 0xa8, 0x72, 0xa9, 0xac, 0xa7, 0x0b, 0x2b, 0xee, 0xbc },
{ 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
{ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46 },
{ 0x1a, 0x91, 0xe1, 0x6f, 0x62, 0xb4, 0xa7, 0xd4, 0x39, 0x54, 0xd6, 0x53, 0x85, 0x95, 0xf7, 0x5e },
{ 0x00, 0xc8, 0x2b, 0xae, 0x95, 0xbb, 0xcd, 0xe5, 0x27, 0x4f, 0x07, 0x69, 0xb2, 0x60, 0xe1, 0x36 },
},

{
{ 0x59, 0x70, 0x47, 0x14, 0xf5, 0x57, 0x47, 0x8c, 0xd7, 0x79, 0xe8, 0x0f, 0x54, 0x88, 0x79, 0x44 },
{ 0x67, 0x53, 0xc9, 0x0c, 0xb7, 0xd8, 0xcd, 0xe5, 0x06, 0xa0, 0x47, 0x78, 0x1a, 0xad, 0x85, 0x11 },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 },
{ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46 },
{ 0x1a, 0x91, 0xe1, 0x6f, 0x62, 0xb4, 0xa7, 0xd4, 0x39, 0x54, 0xd6, 0x53, 0x85, 0x95, 0xf7, 0x5e },
{ 0x00, 0xc8, 0x2b, 0xae, 0x95, 0xbb, 0xcd, 0xe5, 0x27, 0x4f, 0x07, 0x69, 0xb2, 0x60, 0xe1, 0x36 },
},

{

{ 0xd8, 0x2a, 0x91, 0x34, 0xb2, 0x6a, 0x56, 0x50, 0x30, 0xfe, 0x69, 0xe2, 0x37, 0x7f, 0x98, 0x47 },
{ 0x4e, 0xb5, 0x5d, 0x31, 0x05, 0x97, 0x3a, 0x3f, 0x5e, 0x23, 0xda, 0xfb, 0x5a, 0x45, 0xd6, 0xc0 },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00 },
{ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46 },
{ 0x18, 0xc9, 0x1f, 0x6d, 0x60, 0x1a, 0x1a, 0x37, 0x5d, 0x0b, 0x0e, 0xf7, 0x3a, 0xd5, 0x74, 0xc4 },
{ 0x76, 0x32, 0x21, 0x83, 0xed, 0x8f, 0xf1, 0x82, 0xf9, 0x59, 0x62, 0x03, 0x69, 0x0e, 0x5e, 0x01 },

}
};

  int idx, err, x;
  symmetric_LRW lrw;
  unsigned char buf[2][16];

  idx = find_cipher("aes");
  if (idx == -1) {
     idx = find_cipher("rijndael");
     if (idx == -1) {
        return CRYPT_NOP;
     }
  }

  for (x = 0; x < (int)(sizeof(tests)/sizeof(tests[0])); x++) {
     /* schedule it */
     if ((err = lrw_start(idx, tests[x].IV, tests[x].key, 16, tests[x].tweak, 0, &lrw)) != CRYPT_OK) {
        return err;
     }

     /* check pad against expected tweak */
     if (compare_testvector(tests[x].expected_tweak, 16, lrw.pad, 16, "LRW Tweak", x)) {
        lrw_done(&lrw);
        return CRYPT_FAIL_TESTVECTOR;
     }

     /* process block */
     if ((err = lrw_encrypt(tests[x].P, buf[0], 16, &lrw)) != CRYPT_OK) {
        lrw_done(&lrw);
        return err;
     }

     if (compare_testvector(buf[0], 16, tests[x].C, 16, "LRW Encrypt", x)) {
        lrw_done(&lrw);
        return CRYPT_FAIL_TESTVECTOR;
     }

     /* process block */
     if ((err = lrw_setiv(tests[x].IV, 16, &lrw)) != CRYPT_OK) {
        lrw_done(&lrw);
        return err;
     }

     if ((err = lrw_decrypt(buf[0], buf[1], 16, &lrw)) != CRYPT_OK) {
        lrw_done(&lrw);
        return err;
     }

     if (compare_testvector(buf[1], 16, tests[x].P, 16, "LRW Decrypt", x)) {
        lrw_done(&lrw);
        return CRYPT_FAIL_TESTVECTOR;
     }
     if ((err = lrw_done(&lrw)) != CRYPT_OK) {
        return err;
     }
  }
  return CRYPT_OK;
#endif
}

#endif



/* ref:         HEAD -> develop */
/* git commit:  1937f412605e1b04ddb41ef9c2f2f0aab7e61548 */
/* commit time: 2019-11-22 12:03:27 +0100 */
