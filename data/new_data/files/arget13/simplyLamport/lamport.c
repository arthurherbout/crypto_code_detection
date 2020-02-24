/**
    Name        : Why should I name it? I's my son, but actually I hate it ¯\_(ツ)_/¯
                  Okaay, its name is "simplyLamport", happy now?
    Author      : Arget
    Version     : 1.0
    Date        : 15/11/2018
    Description : A simple implementation of the Lamport-Diffie signature
    Notes       : Compile with
                  `gcc -fPIC -shared -o liblamport.so lamport.c'
*/

#include <string.h>
#include "lamport.h"

int genpvk(uint8_t privkey[HASH_SIZE_BITS][2][HASH_SIZE_BYTES],       // OUT
           void (*hashfunc)(uint8_t dst[HASH_SIZE_BYTES],             // IN
                            const uint8_t src[HASH_SIZE_BYTES]))
{
    FILE* f;
    int i;

    // Obtaining random data from a secure source to be used as the private key
    if(!(f = fopen(RANDOMSRC, "r")) ||
       !fread(privkey, PVKSIZE, sizeof(char), f))
    {
        fclose(f);
        return 1;
    }
    fclose(f);

    return 0;
}

void sign(const uint8_t privkey[HASH_SIZE_BITS][2][HASH_SIZE_BYTES],   // IN
          const uint8_t hash[HASH_SIZE_BYTES],                         // IN
          uint8_t signature[SIGNSIZE])                                 // OUT
{
    int i, j;
    uint8_t mask;

    #define BIT (i * BYTE_SIZE + j)
    
    // i counts the byte evaluated inside the message
    for(i = 0; i < HASH_SIZE_BYTES; i++)
    {
        // For each byte we practice 8 iterations of this bucle to check
        // every bit of the byte
        // 'j' counts the bit evaluated inside the byte
        // 'mask' will have a 1 in its j-th bit, so
        // we can AND the byte with mask to know if that bit is set or not
        for(j = 0, mask = 0x80; j < 8; j++, mask /= 2)
            //memcpy(&signature[j * HASH_SIZE_BYTES], privkey[j][(hash[i] & mask)?1:0], HASH_SIZE_BYTES);<OLD
            // BIT is the currently evaluated bit
            memcpy(&signature[BIT * HASH_SIZE_BYTES],
                   privkey[BIT][(hash[i] & mask)?1:0],
                   HASH_SIZE_BYTES);
    }
    #undef BIT
}

int verify(const uint8_t pubkey[HASH_SIZE_BITS][2][HASH_SIZE_BYTES],   // IN
           const uint8_t hash[HASH_SIZE_BYTES],                        // IN
           const uint8_t signature[SIGNSIZE],                          // IN
           void (*hashfunc)(uint8_t dst[HASH_SIZE_BYTES],
                            const uint8_t src[HASH_SIZE_BYTES]))
{
    int i, j;
    uint8_t signature2[SIGNSIZE];

    // Let's compute the hashes of the values from the signature
    #if VERBOSE
    puts("[+] Computing the hashes of the values from the signature");
    #endif
    for(i = 0; i < SIGNSIZE; i += HASH_SIZE_BYTES)
        hashfunc(&signature2[i], &signature[i]);

    // Finally we validate the signature
    #if VERBOSE
    puts("[+] Validating signature . . .");
    #endif
    // Check byte by byte every bit
    // 'i' counts the byte, and 'j' the bit
    for(i = j = 0; i < HASH_SIZE_BYTES; i++, j += 8)
    {
        // Checking bits individually                              // 10000000b
        if(memcmp(signature2+((j+0)*HASH_SIZE_BYTES), pubkey[j+0][(hash[i] & 0x80)?1:0], HASH_SIZE_BYTES) ||
                                                                   // 01000000b
           memcmp(signature2+((j+1)*HASH_SIZE_BYTES), pubkey[j+1][(hash[i] & 0x40)?1:0], HASH_SIZE_BYTES) ||
                                                                   // 00100000b
           memcmp(signature2+((j+2)*HASH_SIZE_BYTES), pubkey[j+2][(hash[i] & 0x20)?1:0], HASH_SIZE_BYTES) ||
                                                                   // 00010000b
           memcmp(signature2+((j+3)*HASH_SIZE_BYTES), pubkey[j+3][(hash[i] & 0x10)?1:0], HASH_SIZE_BYTES) ||
                                                                   // 00001000b
           memcmp(signature2+((j+4)*HASH_SIZE_BYTES), pubkey[j+4][(hash[i] & 0x08)?1:0], HASH_SIZE_BYTES) ||
                                                                   // 00000100b
           memcmp(signature2+((j+5)*HASH_SIZE_BYTES), pubkey[j+5][(hash[i] & 0x04)?1:0], HASH_SIZE_BYTES) ||
                                                                   // 00000010b
           memcmp(signature2+((j+6)*HASH_SIZE_BYTES), pubkey[j+6][(hash[i] & 0x02)?1:0], HASH_SIZE_BYTES) ||
                                                                   // 00000001b
           memcmp(signature2+((j+7)*HASH_SIZE_BYTES), pubkey[j+7][(hash[i] & 0x01)?1:0], HASH_SIZE_BYTES))
        {
            // Somewhere (at j-th byte), something didn't fit,
            // so the signature isn't correct
            return ~i; // If the error is at the 0th byte,
                       // the if in the caller won't detect the error,
                       // so we return the complementary
                       // ~0x0 = 0xffffffff = -1 (signed) = 4294967295 (unsigned)
        }
    }
    return 0;
}

void public(const uint8_t privkey[HASH_SIZE_BITS][2][HASH_SIZE_BYTES], // IN
            uint8_t pubkey[HASH_SIZE_BITS][2][HASH_SIZE_BYTES],        // OUT
            void (*hashfunc)(uint8_t dst[HASH_SIZE_BYTES],             // IN
                             const uint8_t src[HASH_SIZE_BYTES]))
{
    int i;
    for(i = 0; i < HASH_SIZE_BITS; i++)
    {
        hashfunc(pubkey[i][0], privkey[i][0]);
        hashfunc(pubkey[i][1], privkey[i][1]);
    }
}
