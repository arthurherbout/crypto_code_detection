#include "k128.h"

#define R 12

static int f_exp[256];
static int f_log[256];

/**
 * @brief Generates the exp and log tables for the dot operations
 * 
 */
static void generate_dot_tables()
{
    // Calculates the exp[] table
    f_exp[0] = 1;
    for (int i = 1; i < 256; i++)
        f_exp[i] = (f_exp[i - 1] * 45) % 257;
    // Calculates the log[] table 
    f_log[0] = 128;
    for (int i = 0; i < 256; i++)
        f_log[f_exp[i]] = i;
}

/**
 * @brief Checks if the password is valid
 * 
 * @param password The string password
 * @return true    If the number of letters >= 2,
 *                 if the number of numbers >= 2,
 *                 and if the password size >= 8
 * @return false   Otherwise
 */
static bool is_valid_password(char *password)
{
    int letters = 0, digits = 0;
    int pass_length = strlen(password);

    // Counts the number of letters and numbers
    for (int i = 0; i < pass_length; i++)
    {
        char c = password[i];
        if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
            letters++;
        if ('0' <= c && c <= '9')
            digits++;
    }

    // Returns if the password is valid
    return letters >= 2 && digits >= 2 && pass_length >= 8;
}

/**
 * @brief Returns the dot operation between two numbers
 * 
 * @param b The first 64-bit operand
 * @param c The second 64-bit operand
 * @return uint64_t The operation result
 */
static uint64_t dot(uint64_t b, uint64_t c)
{
    uint8_t *A = to_uint8(0);
    uint8_t *B = to_uint8(b);
    uint8_t *C = to_uint8(c);
    for (int i = 0; i < 8; i++)
        A[i] = f_exp[B[i]] ^ f_exp[C[i]];

    return to_uint64(A);
}

/**
 * @brief Returns the inverse dot operation between two numbers
 * such that a = dot(b, c)
 * 
 * @param a The 64-bit operand, result from dot(b, c)
 * @param c The second 64-bit operand from dot(b, c)
 * @return uint64_t The operand b from dot(b, c)
 */
static uint64_t inv_dot(uint64_t a, uint64_t c)
{
    uint8_t *A = to_uint8(a);
    uint8_t *B = to_uint8(0);
    uint8_t *C = to_uint8(c);

    for (int i = 0; i < 8; i++)
        B[i] = f_log[A[i] ^ f_exp[C[i]]];

    return to_uint64(B);
}

/**
 * @brief Returns the complementary of a 64-bit ineteger
 * 
 * @param a The 64-bit integer
 * @return uint64_t The 64-bit integer b such that a + b = 0 mod 2^64
 */
static uint64_t complement(uint64_t a)
{
    return UINT64_MAX - a + 1;
}

/**
 * @brief Executes a XOR operation to a 128-bit block
 * 
 * @param a The first operand and result of the operation
 * @param b The second operand
 */
static void xor_128(uint64_t *a, uint64_t *b)
{
    a[0] ^= b[0];
    a[1] ^= b[1];
}

/**
 * @brief Generates the subskeys from the primary key K
 * 
 * @param K The 128-bit primary key
 * @return uint64_t* The 4*R + 2 64-bit subkeys
 */
uint64_t *generate_subkeys(char *K)
{
    // Allocates the L and k arrays
    uint64_t *L = malloc(sizeof(uint64_t) * (4 * R + 4));
    uint64_t *k = malloc(sizeof (uint64_t) * (4*R + 4));

    // Divides the K primary key
    L[0] = to_uint64((uint8_t *)K);
    L[1] = to_uint64((uint8_t *)(K + 8));
    for (int i = 2; i < 4*R + 3; i++)
        L[i] = L[i-1] + 0x9e3779b97f4a7151;
    
    k[0] = 0x8aed2a6bb7e15162;
    for (int i = 1; i < 4*R + 4; i++)
        k[i] = k[i-1] + 0x7c159e3779b97f4a;
    
    uint64_t A = 0, B = 0;
    for (int s = 1; s < 4*R + 4; s++)
    {
        int i = s - 1;
        k[i] = (k[i] + A + B) << 3;
        A = k[i];
        
        L[i] = (L[i] + A + B) << (A + B);
        B = L[i];
    }

    // Creates the final subkeys array
    uint64_t *subkeys = malloc(sizeof(uint64_t) * (4 * R + 2));
    for (int i = 1; i < 4*R + 3; i++)
        subkeys[i-1] = k[i];
    
    free(L);
    free(k);
    return subkeys;
}

/**
 * @brief Generates the 128-bit primary key 
 * 
 * @param password The string password
 * @return char*  The 128-bit primary key
 */
char *generate_primary_key(char *password)
{
    // Exits if the password is invalid
    if (!is_valid_password(password)) 
    {
        fprintf(stderr, "Wrong password style!");
        exit(EXIT_FAILURE);
    }

    // Allocates the primary key and concatenates the remainder
    char *K = malloc(sizeof (char) * 16);
    int pass_length = strlen(password);
    for (int i = 0; i < 16; i++)
        K[i] = password[i%pass_length];
    return K;
}

/**
 * @brief Executes one encryption iteration
 * 
 * @param X The partial encrypted 128-bit plaintext
 * @param subkeys The array of the four 64-bit subkeys 
 */
void encryption_iteration(uint64_t *X, uint64_t *subkeys) 
{
    // The first part of the iteration
    X[0] = dot(X[0], subkeys[0]);
    X[1] += subkeys[1];
    // The second part of the iteration
    uint64_t Y1 = X[0] ^ X[1];
    uint64_t Y2 = dot(dot(subkeys[2], Y1) + Y1, subkeys[3]);
    uint64_t Z  = dot(subkeys[2], Y1) + Y2;

    X[0] ^= Z;
    X[1] ^= Z;
}

/**
 * @brief Executes a block encryption
 * 
 * @param plaintext_block A 128-bit block of the plaintext
 * @param subkeys The 64-bit subkeys
 */
void block_encryption(uint64_t *plaintext_block, uint64_t *subkeys)
{
    // Executes the encryption iterations
    for (int i = 0; i < R; i++)
        encryption_iteration(plaintext_block, subkeys + 4*i);
    // Executes the final iteration
    plaintext_block[0] = dot(plaintext_block[0], subkeys[4*R]);
    plaintext_block[1] += subkeys[4*R + 1];
}

/**
 * @brief Encrypts the plaintext given by the k128 algorithm
 * 
 * @param plaintext_data The array of bytes to be encrypted
 * @param password The password
 * @param file_size The file's size
 * @param file_size_out Pointer to the size of the output file
 * @return byte_t* The encrypted file
 */
byte_t *encrypt(byte_t *plaintext_data, char *password, uint64_t file_size,
                uint64_t *file_size_out)
{
    // Generates the dot operation tables
    generate_dot_tables();
    // Generates the keys
    char *primary_key = generate_primary_key(password);
    uint64_t *subkeys = generate_subkeys(primary_key);
    free(primary_key);
    // Particionates the data in blocks
    uint64_t **blocks = data_to_blocks(plaintext_data, file_size, true);
    // Calculates the number of blocks
    uint64_t blocks_num = file_size / 16 + (file_size % 16 > 0) + 1;
    // Encrypts each block
    xor_128(blocks[0], uint64_to_block(0xffffffff, 0xffffffff));
    block_encryption(blocks[0], subkeys);
    for (uint64_t i = 1; i < blocks_num; i++)
    {
        xor_128(blocks[i], blocks[i-1]);
        block_encryption(blocks[i], subkeys);
    }
    // Converts the block to an array fo bytes
    byte_t *data = blocks_to_data(blocks, blocks_num, file_size);
    // Calculates the encrypted file size
    *file_size_out = 16*blocks_num + 1;
    // Frees the memory allocated
    free(subkeys);
    free(blocks);
    return data;
}

/**
 * @brief Executes the one decryption iteration
 * 
 * @param Y The partial decrypted ciphertext
 * @param subkeys The array of the four 64-bit subkeys
 */
void decryption_iteration(uint64_t *Y, uint64_t *subkeys)
{
    // The inverse of the second part of the encryption
    // Calculates Y1 from Y[0] = Xe' and Y[1] = Xf'
    // Xe' ^ Xf' = Xe^Z ^ Xf^Z = Xe ^ Xf = Y1 
    uint64_t Y1 = Y[0] ^ Y[1];
    uint64_t Y2 = dot(dot(subkeys[2], Y1) + Y1, subkeys[3]);
    uint64_t Z = dot(subkeys[2], Y1) + Y2;
    // Recovers the original Xe and Xf
    Y[0] ^= Z;
    Y[1] ^= Z;
    
    // The inverse of the first part of the encryption 
    Y[0] = inv_dot(Y[0], subkeys[0]);
    Y[1] += complement(subkeys[1]);
}

/**
 * @brief Executes a block decryption
 * 
 * @param ciphertext_block A 128-bit block of the ciphertext 
 * @param subkeys The 64-bit subkeys
 */
void block_decryption(uint64_t *ciphertext_block, uint64_t *subkeys)
{
    // Executes the final iteration inverse 
    ciphertext_block[0] = inv_dot(ciphertext_block[0], subkeys[4*R]);
    ciphertext_block[1] += complement(subkeys[4*R + 1]);
    // Executes the decryption iterations
    for (int i = R-1; i >= 0; i--)
        decryption_iteration(ciphertext_block, subkeys + 4*i);
}

/**
 * @brief Decrypts the ciphertext given by the k128 inverse algorithm
 * 
 * @param ciphertext_data The array of bytes to be encrypted
 * @param password The password
 * @param file_size The file's size
 * @param file_size_out Pointer to the size of the output file
 * @return byte_t* The file decrypted
 */
byte_t *decrypt(byte_t *ciphertext_data, char *password, uint64_t file_size,
                uint64_t *file_size_out)
{
    // Generates the dot operation tables
    generate_dot_tables();
    // Generates the keys
    char * primary_key = generate_primary_key(password);
    uint64_t *subkeys = generate_subkeys(primary_key);
    // Partionates the data in blocks
    uint64_t **blocks = data_to_blocks(ciphertext_data, file_size, false);
    // Calculates the number of blocks
    uint64_t blocks_num = file_size / 16 + (file_size % 16 > 0);
    // Decrypts each block using the CBC mode
    for (uint64_t i = blocks_num - 1; i > 0; i--)
    {
        block_decryption(blocks[i], subkeys);
        xor_128(blocks[i], blocks[i-1]);
    }
    block_decryption(blocks[0], subkeys);
    xor_128(blocks[0], uint64_to_block(0xffffffff, 0xffffffff));
    // Converts the blocks to an array of bytes
    byte_t *data = blocks_to_data(blocks, blocks_num, file_size);
    // Recovers the original file size
    *file_size_out = to_uint64(data + file_size - 8);
    // Free the memory allocated
    free(primary_key);
    free(subkeys);
    free(blocks);

    return data;
}
