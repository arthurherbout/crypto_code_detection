#pragma once

void AESROUND(unsigned char *out, unsigned char *in, unsigned char *rk);
void AESROUND_INV(unsigned char *out, unsigned char *in, unsigned char *rk);

void aegis128_initialization(const unsigned char *key, const unsigned char *iv, unsigned char *state);
void aegis128_tag_generation(unsigned long long msglen, unsigned long long adlen, unsigned char maclen, unsigned char *mac, unsigned char *state);
void aegis128_enc_aut_step(const unsigned char *plaintextblk, unsigned char *ciphertextblk, unsigned char *state);
void aegis128_dec_aut_step(unsigned char *plaintextblk, const unsigned char *ciphertextblk, unsigned char *state);


#define XOR128(x,y,z) {                                                                             \
    ((unsigned long long*)(x))[0] = ((unsigned long long*)(y))[0] ^ ((unsigned long long*)(z))[0];  \
    ((unsigned long long*)(x))[1] = ((unsigned long long*)(y))[1] ^ ((unsigned long long*)(z))[1];  \
}

#define AND128(x,y,z) {                                                                             \
    ((unsigned long long*)(x))[0] = ((unsigned long long*)(y))[0] & ((unsigned long long*)(z))[0];  \
    ((unsigned long long*)(x))[1] = ((unsigned long long*)(y))[1] & ((unsigned long long*)(z))[1];  \
}

int crypto_aead_encrypt(
	unsigned char *c,unsigned long long *clen,
	const unsigned char *m,unsigned long long mlen,
	const unsigned char *ad,unsigned long long adlen,
	const unsigned char *nsec,
	const unsigned char *npub,
	const unsigned char *k,
	int verbose
);

int crypto_aead_decrypt(
	unsigned char *m,unsigned long long *mlen,
	unsigned char *nsec,
	const unsigned char *c,unsigned long long clen,
	const unsigned char *ad,unsigned long long adlen,
	const unsigned char *npub,
	const unsigned char *k
);
