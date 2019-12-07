#include "utils.h"

/**
 * @brief Converts two 64-bit values in a 128-bit block 
 * 
 * @param a First 64-bit value
 * @param b Second 64-bit value
 * @return uint64_t* The 128-bit block
 */
uint64_t *uint64_to_block(uint64_t a, uint64_t b)
{
    uint64_t *block = malloc(sizeof(uint64_t) * 2);
    block[0] = a;
    block[1] = b;
    return block;
}

/**
 * @brief Converts a 8 bytes array to a 64 bytes integer
 * 
 * @param buffer     The 8-bit array with 8 elements
 * @return uint64_t  The 64-bit integer converted
 */
uint64_t to_uint64(uint8_t *buffer)
{
    uint64_t n = 0;
    // Puts the i-th buffer element in the i-th leftmost byte in n
    for (int i = 0; i < 8; i++)
        n |= (uint64_t)buffer[i] << 8 * (7 - i);
    return n;
}

/**
 * @brief Converts a 64 bytes integer to a 8 bytes array
 * 
 * @param n          The 64-bit integer
 * @return uint8_t * The 8-bit array with 8 elements
 */
uint8_t *to_uint8(uint64_t n)
{
    uint8_t *n_array = malloc(sizeof(int8_t) * 8);
    for (int i = 0; i < 8; i++)
        n_array[i] = n >> 8 * (7 - i);
    return n_array;
}

/**
 * @brief Converts an array of bytes to blocks
 * 
 * @param data The array of bytes
 * @param data_size The size of the data
 * @param is_encrypting If true, appends a extra block with the data size
 * @return uint64_t** The array of blocks
 */
uint64_t **data_to_blocks(byte_t *data, uint64_t data_size,
                                 bool is_encrypting)
{
    // Calculates the data size for a 16-byte multiple plus the data size block
    uint64_t extended_data_size = data_size / 16 + (data_size % 16 > 0) + is_encrypting;
    // Allocates the arrays
    uint64_t **data_blocks = malloc(sizeof(uint64_t *) * extended_data_size);
    uint64_t *data_block;
    for (uint64_t i = 0; i < 16 * extended_data_size; i++)
    {
        // Calculates auxiliary values
        uint64_t block_num = i / 16;
        uint64_t block_byte = i % 16;
        uint64_t block_slice = (block_byte < 8) ? 0 : 1;

        // Creates a block
        if (block_byte == 0)
            data_block = malloc(sizeof(uint64_t) * 2);

        // Initializes an slice of the block
        if (block_byte % 8 == 0)
            data_block[block_slice] = 0;

        // Determines the byte to be allocated
        // Fill with 1 bits incomplete blocks
        byte_t data_byte = 0xff;
        if (i < data_size)
            data_byte = data[i];

        // Puts the byte in the block
        data_block[block_slice] |= (uint64_t)data_byte << 8 * (7 - block_byte % 8);

        // Puts the block in the array
        if (block_byte == 15)
            data_blocks[block_num] = data_block;
    }
    // Appends the data size block
    if (is_encrypting)
        data_blocks[extended_data_size - 1] = uint64_to_block(-1, data_size);
    return data_blocks;
}

/**
 * @brief Converts 128-bit blocks to an array of bytes 
 * 
 * @param blocks The array of 128-bit blocks
 * @param blocks_num The number of blocks
 * @param data_size The size of the data (see is_encrypting)
 * @return byte_t* The array of bytes
 */
byte_t *blocks_to_data(uint64_t **blocks, uint64_t blocks_num,
                              uint64_t data_size)
{
    // Allocates the array of bytes
    byte_t *data = malloc(sizeof(byte_t) * 16 * (blocks_num)); //+ is_encrypting));
    // Puts each byte of the block in the array
    for (uint64_t block_num = 0, i = 0; block_num < blocks_num; block_num++)
        for (int slice = 0; slice < 2; slice++)
            for (int byte = 0; byte < 8; byte++, i++)
                data[i] = blocks[block_num][slice] >> 8 * (7 - byte);

    return data;
}