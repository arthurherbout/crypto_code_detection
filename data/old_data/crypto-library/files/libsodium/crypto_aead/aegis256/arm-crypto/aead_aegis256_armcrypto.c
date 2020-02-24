#include <errno.h>
#include <stdlib.h>
#include <string.h>

#if defined(__ARM_NEON_FP) || defined(__aarch64__)
# include <arm_neon.h>
#endif

static inline void
crypto_aead_aegis256_update(uint8x16_t *const state, const uint8x16_t data)
{
    uint8x16_t tmp;

    tmp      = vaesmcq_u8(vaeseq_u8(state[5], state[0]));
    state[5] = vaesmcq_u8(vaeseq_u8(state[4], state[5]));
    state[4] = vaesmcq_u8(vaeseq_u8(state[3], state[4]));
    state[3] = vaesmcq_u8(vaeseq_u8(state[2], state[3]));
    state[2] = vaesmcq_u8(vaeseq_u8(state[1], state[2]));
    state[1] = vaesmcq_u8(vaeseq_u8(state[0], state[1]));
    state[0] = veorq_u8(tmp, data);
}

static void
crypto_aead_aegis256_init(const unsigned char *key, const unsigned char *iv,
                          uint8x16_t *const state)
{
    static CRYPTO_ALIGN(16) const unsigned char c1[] = {
        0xdd, 0x28, 0xb5, 0x73, 0x42, 0x31, 0x11, 0x20, 0xf1, 0x2f, 0xc2, 0x6d,
        0x55, 0x18, 0x3d, 0xdb
    };
    static CRYPTO_ALIGN(16) const unsigned char c2[] = {
        0x62, 0x79, 0xe9, 0x90, 0x59, 0x37, 0x22, 0x15, 0x0d, 0x08, 0x05, 0x03,
        0x02, 0x01, 0x01, 0x00
    };
    uint8x16_t k1;
    uint8x16_t k2;
    uint8x16_t k3;
    uint8x16_t k4;
    int        i;

    k1 = vld1q_u8(&key[0]);
    k2 = vld1q_u8(&key[16]);
    k3 = veorq_u8(k1, vld1q_u8(&iv[0]));
    k4 = veorq_u8(k2, vld1q_u8(&iv[16]));

    state[0] = k3;
    state[1] = k4;
    state[2] = vld1q_u8(c1);
    state[3] = vld1q_u8(c2);
    state[4] = veorq_u8(k1, state[3]);
    state[5] = veorq_u8(k2, state[2]);

    for (i = 0; i < 4; i++) {
        crypto_aead_aegis256_update(state, k1);
        crypto_aead_aegis256_update(state, k2);
        crypto_aead_aegis256_update(state, k3);
        crypto_aead_aegis256_update(state, k4);
    }
}

static void
crypto_aead_aegis256_mac(unsigned char *mac, unsigned long long mlen,
                         unsigned long long adlen, uint8x16_t *const state)
{
    uint8x16_t tmp;
    int     i;

    tmp = vreinterpretq_u8_u64(vsetq_lane_u64(mlen << 3,
                                              vmovq_n_u64(adlen << 3), 1));
    tmp = veorq_u8(tmp, state[3]);

    for (i = 0; i < 7; i++) {
        crypto_aead_aegis256_update(state, tmp);
    }

    tmp = veorq_u8(state[5], state[4]);
    tmp = veorq_u8(tmp, state[3]);
    tmp = veorq_u8(tmp, state[2]);
    tmp = veorq_u8(tmp, state[1]);
    tmp = veorq_u8(tmp, state[0]);

    vst1q_u8(mac, tmp);
}

static void
crypto_aead_aegis256_enc(unsigned char *const dst,
                         const unsigned char *const src,
                         uint8x16_t *const state)
{
    uint8x16_t msg;
    uint8x16_t tmp;

    msg = vld1q_u8(src);
    tmp = veorq_u8(msg, state[5]);
    tmp = veorq_u8(tmp, state[4]);
    tmp = veorq_u8(tmp, state[1]);
    tmp = veorq_u8(tmp, vandq_u8(state[2], state[3]));
    vst1q_u8(dst, tmp);

    crypto_aead_aegis256_update(state, msg);
}


static void
crypto_aead_aegis256_dec(unsigned char *const dst,
                         const unsigned char *const src,
                         uint8x16_t *const state)
{
    uint8x16_t msg;

    msg = vld1q_u8(src);
    msg = veorq_u8(msg, state[5]);
    msg = veorq_u8(msg, state[4]);
    msg = veorq_u8(msg, state[1]);
    msg = veorq_u8(msg, vandq_u8(state[2], state[3]));
    vst1q_u8(dst, msg);

    crypto_aead_aegis256_update(state, msg);
}

int
crypto_aead_aegis256_encrypt_detached(unsigned char *c, unsigned char *mac,
                                      unsigned long long *maclen_p, const unsigned char *m,
                                      unsigned long long mlen, const unsigned char *ad,
                                      unsigned long long adlen, const unsigned char *nsec,
                                      const unsigned char *npub, const unsigned char *k)
{
    uint8x16_t            state[6];
    CRYPTO_ALIGN(16) unsigned char src[16];
    CRYPTO_ALIGN(16) unsigned char dst[16];
    unsigned long long i;

    (void) nsec;
    crypto_aead_aegis256_init(k, npub, state);

    for (i = 0ULL; i + 16ULL <= adlen; i += 16ULL) {
        crypto_aead_aegis256_enc(dst, ad + i, state);
    }
    if (adlen & 0xf) {
        memset(src, 0, 16);
        memcpy(src, ad + i, adlen & 0xf);
        crypto_aead_aegis256_enc(dst, src, state);
    }
    for (i = 0ULL; i + 16ULL <= mlen; i += 16ULL) {
        crypto_aead_aegis256_enc(c + i, m + i, state);
    }
    if (mlen & 0xf) {
        memset(src, 0, 16);
        memcpy(src, m + i, mlen & 0xf);
        crypto_aead_aegis256_enc(dst, src, state);
        memcpy(c + i, dst, mlen & 0xf);
    }

    crypto_aead_aegis256_mac(mac, mlen, adlen, state);
    sodium_memzero(state, sizeof state);
    sodium_memzero(src, sizeof src);
    sodium_memzero(dst, sizeof dst);

    if (maclen_p != NULL) {
        *maclen_p = 16ULL;
    }
    return 0;
}

int
crypto_aead_aegis256_encrypt(unsigned char *c, unsigned long long *clen_p, const unsigned char *m,
                             unsigned long long mlen, const unsigned char *ad,
                             unsigned long long adlen, const unsigned char *nsec,
                             const unsigned char *npub, const unsigned char *k)
{
    unsigned long long clen = 0ULL;
    int                ret;

    if (mlen > crypto_aead_aegis256_MESSAGEBYTES_MAX) {
        sodium_misuse();
    }
    ret = crypto_aead_aegis256_encrypt_detached(c, c + mlen, NULL, m, mlen,
                                                ad, adlen, nsec, npub, k);
    if (clen_p != NULL) {
        if (ret == 0) {
            clen = mlen + 16ULL;
        }
        *clen_p = clen;
    }
    return ret;
}

int
crypto_aead_aegis256_decrypt_detached(unsigned char *m, unsigned char *nsec, const unsigned char *c,
                                      unsigned long long clen, const unsigned char *mac,
                                      const unsigned char *ad, unsigned long long adlen,
                                      const unsigned char *npub, const unsigned char *k)
{
    uint8x16_t            state[6];
    CRYPTO_ALIGN(16) unsigned char src[16];
    CRYPTO_ALIGN(16) unsigned char dst[16];
    CRYPTO_ALIGN(16) unsigned char computed_mac[16];
    unsigned long long i;
    unsigned long long mlen;
    int                ret;

    (void) nsec;
    mlen = clen;
    crypto_aead_aegis256_init(k, npub, state);

    for (i = 0ULL; i + 16ULL <= adlen; i += 16ULL) {
        crypto_aead_aegis256_enc(dst, ad + i, state);
    }
    if (adlen & 0xf) {
        memset(src, 0, 16);
        memcpy(src, ad + i, adlen & 0xf);
        crypto_aead_aegis256_enc(dst, src, state);
    }
    if (m != NULL) {
        for (i = 0ULL; i + 16ULL <= mlen; i += 16ULL) {
            crypto_aead_aegis256_dec(m + i, c + i, state);
        }
    } else {
        for (i = 0ULL; i + 16ULL <= mlen; i += 16ULL) {
            crypto_aead_aegis256_dec(dst, c + i, state);
        }
    }
    if (mlen & 0xf) {
        memset(src, 0, 16);
        memcpy(src, c + i, mlen & 0xf);
        crypto_aead_aegis256_dec(dst, src, state);
        if (m != NULL) {
            memcpy(m + i, dst, mlen & 0xf);
        }
        memset(dst, 0, mlen & 0xf);
        state[0] = veorq_u8(state[0], vld1q_u8(dst));
    }

    crypto_aead_aegis256_mac(computed_mac, mlen, adlen, state);
    sodium_memzero(state, sizeof state);
    sodium_memzero(src, sizeof src);
    sodium_memzero(dst, sizeof dst);
    ret = crypto_verify_16(computed_mac, mac);
    sodium_memzero(computed_mac, sizeof computed_mac);
    if (m == NULL) {
        return ret;
    }
    if (ret != 0) {
        memset(m, 0, mlen);
        return -1;
    }
    return 0;
}

int
crypto_aead_aegis256_decrypt(unsigned char *m, unsigned long long *mlen_p, unsigned char *nsec,
                             const unsigned char *c, unsigned long long clen,
                             const unsigned char *ad, unsigned long long adlen,
                             const unsigned char *npub, const unsigned char *k)
{
    unsigned long long mlen = 0ULL;
    int                ret  = -1;

    if (clen >= 16ULL) {
        ret = crypto_aead_aegis256_decrypt_detached(m, nsec, c, clen - 16ULL, c + clen - 16ULL, ad,
                                                    adlen, npub, k);
    }
    if (mlen_p != NULL) {
        if (ret == 0) {
            mlen = clen - 16ULL;
        }
        *mlen_p = mlen;
    }
    return ret;
}
