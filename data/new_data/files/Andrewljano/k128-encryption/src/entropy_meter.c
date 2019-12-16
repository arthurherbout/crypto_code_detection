#include "entropy_meter.h"

/**
 * @brief Changes one (or two) bits from an array of bytes
 * 
 * @param dest Pointer to the destination array where the content is to be copied.
 * @param src Original array of bytes
 * @param bit_index Index of the bit to be modified  
 * @param data_size The data's sizze    
 * @param mode If mode == 2, also alterates the (bit_index + 8)-th bit
 */
static void change_data(byte_t *dest, byte_t *src, uint64_t bit_index,
                        uint64_t data_size, int mode)
{
    // Copies the data source to the destination array
    for (uint64_t i = 0; i < data_size; i++)
        dest[i] = src[i];
    
    // Alterates the (bit_index)-th bit  
    dest[bit_index / 8] ^= (1 << (7 - bit_index % 8));

    // If mode == 2, also alterates the (bit_index + 8)-th bit
    if (mode == 2 && bit_index + 8 < 8 * data_size)
        dest[bit_index / 8 + 1] ^= 1 << (7 - bit_index % 8);
}

/**
 * @brief Calculates the hamming_distance from blocks of two arrays
 * 
 * @param data The original data
 * @param changed_data The changed data
 * @return int The hamming distance
 */
static int hamming_distance(byte_t *data, byte_t *changed_data)
{
    int distance = 0;
    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 8; j++)
            distance += ((data[i] >> j) & 1) ^ ((changed_data[i] >> j) & 1);
    return distance;
}

/**
 * @brief Calculates the entropy of the k126 algorithm
 * 
 * @param input The array of bytes of the input file
 * @param password The password to the encryption
 * @param file_size The file's size
 * @param mode The mode of the calculation (see change_data function)
 */
void entropy_meter(byte_t *input, char *password, uint64_t file_size, int mode)
{
    uint64_t file_size_out;
    byte_t *input_changed = malloc(sizeof(byte_t) * file_size);
    // Encrypts the input file
    byte_t *enc_input = encrypt(input, password, file_size, &file_size_out);

    uint64_t num_blocks = file_size_out/16 - 1;
    // Creates 5 auxiliary arrays for the calculations
    uint64_t *blocks_distance[5];
    for (int sum_type = 0; sum_type < 5; sum_type++)
    {
        blocks_distance[sum_type] = malloc(sizeof(uint64_t) * num_blocks);
        for (uint64_t i = 0; i < num_blocks; i++)
            blocks_distance[sum_type][i] = (sum_type == B_MIN) ? 128 : 0;
    }        
    
    for (uint64_t i = 0; i < 8 * file_size_out - 128; i++)
    {
        // Changes the data and encrypts the changed data 
        change_data(input_changed, input, i, file_size, mode);
        byte_t *enc_changed = encrypt(input_changed, password, file_size, 
                                      &file_size_out);
        for (uint64_t block = 0; block < num_blocks; block++)
            if (i / 128 <= block)
            {
                // Calculates the distance of one block
                int distance = hamming_distance(enc_input + 16 * block,
                                                enc_changed + 16 * block);
                // Finds the partial maximum distance
                if (distance > blocks_distance[B_MAX][block])
                    blocks_distance[B_MAX][block] = distance;
                // Finds the partial minimum distance  
                if (distance < blocks_distance[B_MIN][block])
                    blocks_distance[B_MIN][block] = distance;
                // Sums the distance
                blocks_distance[B_SUM][block] += distance;
                blocks_distance[B_SQD][block] += distance * distance;
                blocks_distance[B_TOT][block] += 1;
            }
        free(enc_changed);                                                           
    }

    // Calculates the values for each block and prints the table
    printf("Mode %d\n", mode);
    printf("          \tMean distance\tMax distance\tMin distance\tStd Deviation\n");
    for (uint64_t i = 0; i < num_blocks; i++)
    {
        uint64_t total = blocks_distance[B_TOT][i];
        uint64_t sum = blocks_distance[B_SUM][i]; 
        float mean = sum / ((i + 1) * 128.0);
        uint64_t sum_sqd = blocks_distance[B_SQD][i]; 
        double std_dev = sqrt((sum_sqd - 2 * mean * sum + total * mean * mean)/(total - 1.0));
        printf("Block %5lu\t%11.2f\t%12lu\t%12lu\t%12.2f\n", i + 1,
                mean,
                blocks_distance[B_MAX][i],
                blocks_distance[B_MIN][i],
                std_dev);
    }
    
    // Frees the auxiliary arrays
    for (int i = 0; i < 5; i++)
        free(blocks_distance[i]);
}