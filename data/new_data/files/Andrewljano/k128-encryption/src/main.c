#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "k128.h"
#include "io_handling.h"
#include "entropy_meter.h"

#define MAX_PASSWORD_SIZE 16

/**
 * @brief Overwrite with 0's and erases the file given
 * 
 * @param filename The file's name
 * @param file_size The file's size
 */
static void erase_file(char *filename, uint64_t file_size)
{
    // Creates an array with 0's
    byte_t *empty_file = malloc(sizeof(byte_t) * file_size);
    for (int i = 0; i < file_size; i++)
        empty_file[i] = 0;
    // Overwrites the file
    write_array_to_file(filename, empty_file, file_size);
    
    // Removes the file 
    if (remove(filename)) 
    {
        printf("Unable to remove the file");
        exit(1);
    }
}

int main(int argc, char **argv) 
{
    
    int mode = -1;
    char filename_in[MAX_FILE_NAME_SIZE + 1];
    char filename_out[MAX_FILE_NAME_SIZE + 1];
    char password[MAX_PASSWORD_SIZE + 1];
    bool do_erase_file = false;

    if (argc < 6)
    {
        printf("Wrong arguments given");
        exit(1);
    }

    // Gets the execution mode
    mode = argv[1][1];
    // Copies the input filename
    strcpy(filename_in, argv[3]);

    if (mode == 'c' || mode == 'd')
    {
        if (argc < 8)
        {
            printf("Wrong arguments given");
            exit(1);
        }
        // Copies the output filename
        strcpy(filename_out, argv[5]);
        // Copies the password
        strcpy(password, argv[7]);
        // Calculates the erase_file flag
        do_erase_file = argc == 9 && strcmp(argv[8], "-a") == 0;
    }
    else
        // Copies the password 
        strcpy(password, argv[5]);
   
    uint64_t file_size; 
    uint64_t file_size_out;
    byte_t *input_file;
    byte_t *output_file;

    // Gets the file's size
    file_size = get_file_size(filename_in);
    // Reads the input file to an array
    input_file = malloc(sizeof(byte_t) * file_size);
    read_file_to_array(filename_in, input_file, file_size);
    
    switch (mode)
    {
        case 'c':;
            // Encrypts the file to an array
            output_file = encrypt(input_file, password, file_size, &file_size_out);
            // Writes the array in the output file
            write_array_to_file(filename_out, output_file, file_size_out-1);
            free(input_file);
            free(output_file);

            if (do_erase_file) 
                erase_file(filename_in, file_size);
            break;
        case 'd':;
            // Decrypts the file to an array 
            output_file = decrypt(input_file, password, file_size, &file_size_out);
            // Writes the array in the output file
            write_array_to_file(filename_out, output_file, file_size_out);
            free(input_file);
            free(output_file);
            break;
        case '1':;
            // Calculates the mode 1 entropy 
            entropy_meter(input_file, password, file_size, 1);
            break;
        case '2':;
            // Calculates the mode 2 entropy 
            entropy_meter(input_file, password, file_size, 2);
            break;
    }

    return 0;
}