/**
    Name        : Just a test of simplyLamport
    Author      : Arget
    Version     : 1.2
    Date        : 15/11/2018
    Description : A simple implementation of the Lamport-Diffie signature
    Notes       : Compile with
                  `gcc -L/path/to/liblamport.so -o lamport test.c base64.c -lcrypto -llamport'
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <openssl/sha.h>
#define HASHALGO SHA256
int hashSizeBits = 256;

//#include <openssl/md5.h>
//#define HASHALGO MD5
//int hashSizeBits = 128;

#include "test.h"
#include "base64.h"

int main(int argc, char** argv)
{
    // Multipurpose variables
    FILE* f;
    int i;

    // "Flags"-variable determined by the parameters
    uint8_t action;

    // Used in the signing process
    char* msgfile;


    // Will content the hash of the message
    uint8_t hash[HASH_SIZE_BYTES];

    // Random data, to be used as the private key
    //uint8_t privkey[HASH_SIZE_BITS][2][HASH_SIZE_BYTES];

    // Hashes of each 'HASH_SIZE_BYTES' bytes in the previous matrix,
    // to be used as public key
    //uint8_t pubkey[HASH_SIZE_BITS][2][HASH_SIZE_BYTES];

    // Will contain the key we are gonna use, private or public
    uint8_t key[HASH_SIZE_BITS][2][HASH_SIZE_BYTES];
    
    // The final signature of the message's hash
    uint8_t signature[HASH_SIZE_BITS * HASH_SIZE_BYTES];


    // Initializing variable¿s?
    progname = argv[0];

    // Obv.
    if(argc < 2)
        usage("[!] At least one argument required");


    /** Parsing arguments **/
    if(argv[1][0] == '-') // The 1st argument should start with '-'
    {
        if(!argv[1][1])   // The 1st argument is _just_ "-"
        {
            action = 1;
            msgfile = NULL; // The message comes from stdin
        }
        else
        {
            switch(argv[1][1])
            {
                case 's': // sign
                    if(argc < 4)
                        usage("[!] msgfile missing");
                    action = 1;
                    msgfile = argv[3];
                    break;
                case 'g': // genkeys
                    action = 2;
                    break;
                case 'v': // verify
                    action = 4;
                    break;
                case 'p': // public
                    action = 8;
                    break;
                case 'h': // help
                    usage(NULL);
                    //break;  Unnecessary, usage() doesn't return (*snif*)

                default:
                    usage("[!] Invalid parameter");
            }
        }
    }
    else // I need the user to tell me what the fuck should I do
         // So I'll have to teach her/him to use this shitty program
        usage("[!] You don't know how to use this program, so...");
    
    if(action & 0x1) // Signing
    {
        if(argc < 3)
            usage("[!] I need the privfile");

        // Read the private key from the provided privfile
        printf("[+] Reading the private key from %s\n", argv[2]);
        readkey(argv[2], key);

        // Obtaining the hash of the message, wherever it comes from
        HASHFILE(msgfile, hash);
        
        // Performing the "calculation" of the signature (a very very *hard* process)
        puts("[+] Signing the message . . .");
        sign(key, hash, signature);

        // If the user specified outfile, we write there the signature
        // instead of printing it in base64 to stdout
        if((!msgfile && argc > 3) || (msgfile && argc > 4))
        {
            printf("[+] Writing signature to %s\n", argv[(!msgfile)?3:4]);
            tofile((uint8_t*)signature, argv[(!msgfile)?3:4], sizeof signature);
        }
        else // Otherwise, print the base64 encoded signature
        {
            puts("-----BEGIN LAMPORT SIGNATURE-----");
            printf("Hash algorithm: %s\n", HASHALGOSTR);
            tofile((uint8_t*)signature, NULL, sizeof signature);
            puts("\n-----END LAMPORT SIGNATURE-----");
        }
    }
    else
    if(action & 0x2) // Generate keypair
    {
        if(argc > 2 && argc < 4)
            usage("[!] You can't specify only outfile1.\n"
                  "    I need you to specify both outfile1 (for the private key)\n"
                  "    and outfile2 (for the public one), or none");

        // This function will give us the keypair
        puts("[+] Calculating Lamport keypair . . .");
        
        puts("[+] Obtaining random data from a secure source (may take a while)");
        if(genpvk(key, &HASH))
            panic("Error in genpvk() reading from the random source", -1, RANDOMSRC);

        if(argc > 2) // If the user specified the outfiles
        {
            printf("[+] Writing private key to %s\n", argv[2]);
            tofile((uint8_t*)key, argv[2], sizeof key);
        }
        else // Print the private key in base64
        {
            puts("[+] Here you have your keys. Enjoy!\n\n");
            puts("-----BEGIN LAMPORT PRIVATE KEY BLOCK-----");
            printf("Hash algorithm: %s\n", HASHALGOSTR);
            tofile((uint8_t*)key, NULL, sizeof key);
            puts("\n-----END LAMPORT PRIVATE KEY BLOCK-----\n\n");
        }

        // Got the private key. Let's compute the public one
        puts("[+] Computing the public key from the private one");
        public(key, key, &HASH);

        if(argc > 2)
        {
            printf("[+] Writing public key to %s\n", argv[3]);
            tofile((uint8_t*)key, argv[3], sizeof key);
        }
        else
        {
            puts("\n\n-----BEGIN LAMPORT PUBLIC KEY BLOCK-----");
            printf("Hash algorithm: %s\n", HASHALGOSTR);
            tofile((uint8_t*)key, NULL, sizeof key);
            puts("\n-----END LAMPORT PUBLIC KEY BLOCK-----");
        }
    }
    else
    if(action & 0x4) // Verifying a signature
    {
        if(argc < 5)
            usage("[!] Missing arguments");

        // Read the private key from the provided privfile
        printf("[+] Reading the public key from %s\n", argv[2]);
        readkey(argv[2], key);

        // Now, read the signature from the signfile
        printf("[+] Reading the signature from %s\n", argv[3]);
        if((!(f = fopen(argv[3], "r"))) ||
           (!fread(signature, sizeof signature, sizeof(char), f)))
            panic("Error reading from the file or it's too small", -1, argv[3]);
        fclose(f);

        // We need the hash of the message
        HASHFILE(argv[4], hash);

        // And now validate the signature
        // (I'm gonna reuse the 'i' var, nobody will miss her...)
        if((i = verify(key, hash, signature, &HASH)))
            printf("[!] Found an error in byte %d\n"
                   "[!] INVALID signature.\n"
                   "LOL Something strange happened...\n"
                   " ... or someone is doing something nasty", (~i) + 1); ///////!!!!!!!!!!!
        else
            printf("[*] VALID signature\n"
                   "The message's integrity is ensured, as long as the issuer's\n"
                   "  private key has not been compromised, who knows?");
    }
    else
    if(action & 0x8) // Obtain the public key given a private key
    {
        if(argc < 3)
            usage("[!] I need the privfile");
        
        // Read the private key from the provided privfile
        printf("[+] Reading the private key from %s\n", argv[2]);
        readkey(argv[2], key);

        // Now let's compute the public key
        puts("[+] Computing the public key from the private one");
        public(key, key, &HASH);

        if(argc < 4) // If the user hasn't gave me a file in which write to
        {
            puts("[+] Here you have your public key. Enjoy! c:\n\n");

            puts("-----BEGIN LAMPORT PUBLIC KEY BLOCK-----");
            printf("Hash algorithm: %s\n", HASHALGOSTR);
            tofile((uint8_t*)key, NULL, sizeof key);
            puts("\n-----END LAMPORT PUBLIC KEY BLOCK-----");
        }
        else
        {
            printf("[+] Writing public key to %s\n", argv[3]);
            tofile((uint8_t*)key, argv[3], sizeof key);
        }
    }
    
    putchar('\n');
    
    return 0;
}

void readkey(const uint8_t* filename,
             uint8_t keyarray[HASH_SIZE_BITS][2][HASH_SIZE_BYTES])
{
    FILE* f;

    if(!(f = fopen(filename, "r")))
        panic("Error opening the file", -1, filename);
    
    if(fread(keyarray, 1, (HASH_SIZE_BITS * HASH_SIZE_BYTES * 2), f)
       != (HASH_SIZE_BITS * HASH_SIZE_BYTES * 2))
        panic("Keyfile too small", 0, filename);

    fclose(f);
}

void panic(const char* s, const int code,
           const char* justonemorething)
{
    fprintf(stderr, "[!] ");
    if(!justonemorething) fprintf(stderr, "%s: ", progname);
    else                  fprintf(stderr, "%s: \"%s\": ", progname, justonemorething);
    if(code)
    {
        perror(s);
        exit(code);
    }
    //else    // exit() will never return, deal with it
    fprintf(stderr, s);
    exit(1);
}

void usage(const char* s)
{
    if(s) puts(s);

    printf("Usage:\n"
           "\tGenerate a keypair\n"
           "\t  %1$s -g [outfile1 outfile2]\n\n"

           "\tSign\n"
           "\t  %1$s -s privfile msgfile [outfile]\n"
           "\t  %1$s -  privfile [outfile]\n\n"
    
           "\tVerify a signature\n"
           "\t  %1$s -v pubfile signfile msgfile\n\n"
       
           "\tGiven a private key, obtain the public key\n"
           "\t  %1$s -p privfile [outfile]\n\n"

           "\tPrint this message\n"
           "\t  %1$s -h\n\n"

           "Note:\n"
           "\tprivfile and pubfile must contain in raw the private and public keys,\n"
           "\t  respectively.\n"
           "\toutfile will content the resulting data in raw format.\n", progname);
    exit(1);
}

void HASH(uint8_t dst[HASH_SIZE_BYTES], const uint8_t src[HASH_SIZE_BYTES])
{
    EVALUATOR(HASHALGO, _CTX) ctx;

    EVALUATOR(HASHALGO, _Init)(&ctx);
    EVALUATOR(HASHALGO, _Update)(&ctx, src, HASH_SIZE_BYTES);
    EVALUATOR(HASHALGO, _Final)(dst, &ctx);
}

void HASHFILE(const uint8_t* const filename, uint8_t hash[HASH_SIZE_BYTES])
{
    uint8_t readbuffer[BUFSIZE];
    size_t r;
    FILE* f;
    EVALUATOR(HASHALGO, _CTX) ctx;

    EVALUATOR(HASHALGO, _Init)(&ctx);
    if(filename)
    {
        printf("[+] Reading from %s\n[+] Computing the hash...", filename);
        fflush(stdout);
        
        if(!(f = fopen(filename, "r")))
            panic("Error opening the file specified", -1, filename);
        while(!feof(f))
            EVALUATOR(HASHALGO, _Update)(&ctx, readbuffer, fread(readbuffer, 1, BUFSIZE, f));
        fclose(f);
    }
    else
    {
        puts("[+] Reading from stdin, go ahead, type your message, don't be afraid");
        while(r = fread(readbuffer, 1, BUFSIZE, stdin))
            EVALUATOR(HASHALGO, _Update)(&ctx, readbuffer, r);
        printf("[+] Computing the hash... ");
        fflush(stdout);
    }

    EVALUATOR(HASHALGO, _Final)(hash, &ctx);

    // Print the hash just calculated
    printf(" Hash of the message (%s):\n", HASHALGOSTR);
    for(r = 0; r < HASH_SIZE_BYTES; r++)
        printf("%02x", hash[r]);
    puts("\n");
}

void tofile(const uint8_t* const data,
            const uint8_t* const filename,
            const size_t len)
{
    FILE* f;
    uint8_t* bs64;
    size_t bs64len;
    int i;

    if(filename)
    {
        if((!(f = fopen(filename, "w"))) ||
           (!fwrite(data, len, sizeof(char), f)))
            panic("Error writing to the file specified", -1, filename);
        fclose(f);
    }
    else
    {
        bs64len = Base64encode_len(len);
        if(!(bs64 = malloc(bs64len)))
            panic("Error allocating memory", -1, NULL);
        Base64encode(bs64, data, len);
        for(i = 0; i < bs64len; i += 64)
            printf("\n%.64s", &bs64[i]);
        free(bs64);
    }
}
