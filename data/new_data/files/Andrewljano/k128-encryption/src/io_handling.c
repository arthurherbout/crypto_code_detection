#include "io_handling.h"

/**
 * @brief Gets the file size object in bytes
 * 
 * @param file_name The file name
 * @return unsigned long long The file size
 */
uint64_t get_file_size(char file_name[]) 
{
    FILE *p_input_file;
    uint64_t file_size;

    p_input_file = fopen(file_name, "rb");
    if (p_input_file == NULL) 
    {
        printf("Input file %s not found.\n", file_name);
        exit(1);
    }

    fseek(p_input_file, 0, SEEK_END);
    file_size = ftell(p_input_file);
    fseek(p_input_file, 0, SEEK_SET);
    fclose(p_input_file);

    return file_size;
}

/**
 * @brief Reads a file to an array of bytes
 * 
 * @param file_name The file name
 * @param file_bytes The array of bytes
 * @param file_size The file size
 */
void read_file_to_array(char file_name[], byte_t file_bytes[], uint64_t file_size) 
{
    FILE *p_input_file;

    p_input_file = fopen(file_name, "rb");
    if (p_input_file == NULL) 
    {
        printf("Input file %s not found.\n", file_name);
        exit(1);
    }

    fread(file_bytes, sizeof(*file_bytes), file_size, p_input_file);
    fclose(p_input_file);
}

/**
 * @brief Writes an array of bytes to a file
 * 
 * @param file_name The file name
 * @param file_bytes The array of bytes
 * @param file_size The file size
 */
void write_array_to_file(char file_name[], byte_t file_bytes[], uint64_t file_size) 
{
    FILE *p_output_file;

    p_output_file = fopen(file_name, "wb");
    if (p_output_file == NULL) 
    {
        printf("Output file %s not found.\n", file_name);
        exit(1);
    }
    
    fwrite(file_bytes, sizeof(*file_bytes), file_size, p_output_file);
    
    fclose(p_output_file);
}