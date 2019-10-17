#ifndef YAES128V2_H
#define YAES128V2_H

#include "../../../CaesarInterface.h"

class Yaes128v2 : public CaesarInterface {
    const int maxNumRounds = -1;
public:
    Yaes128v2(int numRounds);
    ~Yaes128v2();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // YAES128V2_H
