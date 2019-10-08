///////////////////////////////////////////////
//
// **************************
// ** ENGLISH - 10/Jul/2017 **
//
// Project: libObfuscate v2.00
//
// This software is released under:
// * LGPL 3.0: "www.gnu.org/licenses/lgpl.html"
//
// You’re free to copy, distribute and make commercial use
// of this software under the following conditions:
// * You cite the author and copyright owner: "www.embeddedsw.net"
// * You provide a link to the Homepage: "www.embeddedsw.net/libobfuscate.html"
//
///////////////////////////////////////////////

#define DLLIMP __declspec(dllimport)

#include "libObfuscateDef.h"

///////////////////////////////////////////////////////
// Base cryptography

DLLIMP	void Anubis_set_key(ANUBIS_DATA *pAd,const BYTE *key);
DLLIMP	void Anubis_encrypt(const ANUBIS_DATA *pAd,const BYTE *plaintext,BYTE *ciphertext);
DLLIMP	void Anubis_decrypt(const ANUBIS_DATA *pAd,const BYTE *ciphertext,BYTE *plaintext);

DLLIMP	void Camellia_set_key(BYTE *e,const int n,const BYTE *k);
DLLIMP	void Camellia_encrypt(const BYTE *e,const int n,const BYTE *p,BYTE *c);
DLLIMP	void Camellia_decrypt(const BYTE *e,const int n,const BYTE *c,BYTE *p);

DLLIMP	void Cast256_set_key(DWORD *l_key,const DWORD *in_key, const DWORD key_len);
DLLIMP	void Cast256_encrypt(const DWORD *l_key,const DWORD *in_blk, DWORD *out_blk);
DLLIMP	void Cast256_decrypt(const DWORD *l_key,const DWORD *in_blk, DWORD *out_blk);

DLLIMP	void Clefia_set_key(CLEFIA_DATA *pCd,const BYTE *skey,const int key_bitlen);
DLLIMP	void Clefia_encrypt(const CLEFIA_DATA *pCd,const BYTE *pt,BYTE *ct);
DLLIMP	void Clefia_decrypt(const CLEFIA_DATA *pCd,const BYTE *ct,BYTE *pt);

DLLIMP	void Frog_set_key(FROG_DATA *pFd,const DWORD *in_key,const DWORD key_len);
DLLIMP	void Frog_encrypt(const FROG_DATA *pFd,const DWORD in_blk[4], DWORD out_blk[4]);
DLLIMP	void Frog_decrypt(const FROG_DATA *pFd,const DWORD in_blk[4], DWORD out_blk[4]);

DLLIMP	void Hierocrypt3_set_key(HIEROCRYPT3_DATA *pHd,const BYTE* key);
DLLIMP	void Hierocrypt3_encrypt(const HIEROCRYPT3_DATA *pHd,const BYTE * in,BYTE *out);
DLLIMP	void Hierocrypt3_decrypt(const HIEROCRYPT3_DATA *pHd,const BYTE * in,BYTE *out);

DLLIMP	void Ideanxt128_set_key(nxt128_ctx *ctx,const BYTE *key,const WORD key_len);
DLLIMP	void Ideanxt128_encrypt(const nxt128_ctx *ctx,const BYTE *in,BYTE *out);
DLLIMP	void Ideanxt128_decrypt(const nxt128_ctx *ctx,const BYTE *in,BYTE *out);

DLLIMP	void Mars_set_key(DWORD *l_key,DWORD *vk,const DWORD *in_key, const DWORD key_len);
DLLIMP	void Mars_encrypt(const DWORD *l_key,const DWORD *in_blk, DWORD *out_blk);
DLLIMP	void Mars_decrypt(const DWORD *l_key,const DWORD *in_blk, DWORD *out_blk);

DLLIMP	void Rc6_set_key(DWORD *l_key,const DWORD *in_key, const DWORD key_len);
DLLIMP	void Rc6_encrypt(const DWORD *l_key,const DWORD *in_blk, DWORD *out_blk);
DLLIMP	void Rc6_decrypt(const DWORD *l_key,const DWORD *in_blk, DWORD *out_blk);

DLLIMP	int Rijndael_set_key_encrypt(DWORD *rk,const BYTE *key,const int keybits);
DLLIMP	int Rijndael_set_key_decrypt(DWORD *rk,const BYTE *key,const int keybits);
DLLIMP	void Rijndael_encrypt(const DWORD *rk,const int nrounds,const BYTE *plaintext,BYTE *ciphertext);
DLLIMP	void Rijndael_decrypt(const DWORD *rk,const int nrounds,const BYTE *ciphertext,BYTE *plaintext);

DLLIMP	void Saferp_set_key(SAFERP_DATA *pSpd,const DWORD *in_key, const DWORD key_len);
DLLIMP	void Saferp_encrypt(const SAFERP_DATA *pSpd,const DWORD *in_blk, DWORD *out_blk);
DLLIMP	void Saferp_decrypt(const SAFERP_DATA *pSpd,const DWORD *in_blk, DWORD *out_blk);

DLLIMP	void Sc2000_set_key(DWORD *ek,const DWORD *in_key);
DLLIMP	void Sc2000_encrypt(const DWORD *ek,const DWORD *in, DWORD *out);
DLLIMP	void Sc2000_decrypt(const DWORD *ek,const DWORD *in, DWORD *out);

DLLIMP	void Serpent_set_key(DWORD *l_key,const DWORD *in_key, const DWORD key_len);
DLLIMP	void Serpent_encrypt(const DWORD *l_key,const DWORD *in_blk, DWORD *out_blk);
DLLIMP	void Serpent_decrypt(const DWORD *l_key,const DWORD *in_blk, DWORD *out_blk);

DLLIMP	void Speed_set_key(speed_ikey rndkey,const speed_key key);
DLLIMP	void Speed_encrypt(const speed_ikey rndkey,const speed_data pt,speed_data ct);
DLLIMP	void Speed_decrypt(const speed_ikey rndkey,const speed_data ct,speed_data pt);

DLLIMP	void Twofish_set_key(TWOFISH_DATA *pTfd,const DWORD *in_key, const DWORD key_len);
DLLIMP	void Twofish_encrypt(const TWOFISH_DATA *pTfd,const DWORD *in_blk, DWORD *out_blk);
DLLIMP	void Twofish_decrypt(const TWOFISH_DATA *pTfd,const DWORD *in_blk, DWORD *out_blk);

DLLIMP	void Unicorn_set_key(BYTE *eKey,const BYTE *secret);
DLLIMP	void Unicorn_encrypt(const BYTE *eKey,const BYTE *src,BYTE *dst);
DLLIMP	void Unicorn_decrypt(const BYTE *eKey,const BYTE *src,BYTE *dst);

///////////////////////////////////////////////////////
// Base hashing

DLLIMP	void Grostl512_init(GROSTL512_DATA *grostl);
DLLIMP	void Grostl512_data(GROSTL512_DATA *grostl,const BYTE *buffer,DWORD len);
DLLIMP	void Grostl512_finalize(GROSTL512_DATA *grostl,BYTE *hash);

DLLIMP	void Keccak512_init(KECCAK512_DATA *keccak);
DLLIMP	void Keccak512_data(KECCAK512_DATA *keccak,const BYTE *buffer,DWORD len);
DLLIMP	void Keccak512_finalize(KECCAK512_DATA *keccak,BYTE *hash);

DLLIMP	void Sha512_init(SHA512_DATA *sha);
DLLIMP	void Sha512_data(SHA512_DATA *sha,const void *buffer,DWORD len);
DLLIMP	void Sha512_finalize(SHA512_DATA *sha,BYTE *hash);

DLLIMP	void Skein512_init(SKEIN512_DATA *skein);
DLLIMP	void Skein512_data(SKEIN512_DATA *skein,const BYTE *buffer,DWORD len);
DLLIMP	void Skein512_finalize(SKEIN512_DATA *skein,BYTE *hash);

///////////////////////////////////////////////////////
// Simple cryptography wrapping

DLLIMP	void Multi_single_setkey(MULTI_STATIC_DATA *cd,const ENUM_ALG curAlg,const BYTE *passw);
DLLIMP	void Multi_ECB_single_encrypt(const MULTI_STATIC_DATA *mSd,const ENUM_ALG curAlg,const BYTE *inBuf,BYTE *outBuf);
DLLIMP	void Multi_ECB_single_decrypt(const MULTI_STATIC_DATA *mSd,const ENUM_ALG curAlg,const BYTE *inBuf,BYTE *outBuf);

///////////////////////////////////////////////////////
// CSPRNG

DLLIMP	void CSPRNG_set_seed(CSPRNG_DATA *pCd,ENUM_HASH hashE,const BYTE *passw,DWORD nonce);
DLLIMP	void CSPRNG_autoseed(CSPRNG_DATA *pCd,perc_callback_t backFunc,void *desc);

DLLIMP	BYTE CSPRNG_get_byte(CSPRNG_DATA *pCd);
DLLIMP	WORD CSPRNG_get_word(CSPRNG_DATA *pCd);
DLLIMP	DWORD CSPRNG_get_dword(CSPRNG_DATA *pCd);
DLLIMP	OBFUNC_RETV CSPRNG_randomize(CSPRNG_DATA *pCd,const DWORD len,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);
DLLIMP	void CSPRNG_array_init(CSPRNG_DATA *pCd,DWORD max,BYTE *buf);

///////////////////////////////////////////////////////
// Multi-cryptography

DLLIMP	void Multi_setkey(MULTI_DATA *pMd,const BYTE *iv,const BYTE *passw1,const BYTE *passw2,DWORD nonce);

DLLIMP	OBFUNC_RETV Multi_CBC_encrypt(MULTI_DATA *pMd,const DWORD len,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);
DLLIMP	OBFUNC_RETV Multi_CBC_decrypt(MULTI_DATA *pMd,const DWORD len,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);

///////////////////////////////////////////////////////
// Scramble

DLLIMP	OBFUNC_RETV Scramble_seed(SCRAMBLE_DATA *pSd,const DWORD len,const BYTE *passw,DWORD nonce);
DLLIMP	void Scramble_end(SCRAMBLE_DATA *pSd);

DLLIMP	OBFUNC_RETV Seg_scramble(SCRAMBLE_DATA *pSd,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);
DLLIMP	OBFUNC_RETV Seg_descramble(SCRAMBLE_DATA *pSd,BYTE *buf,perc_callback_t pFunc,void *pDesc,test_callback_t tFunc,void *tDesc);
