#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/******************************
 * MD2 implementation in C
 * Can test on liberty.txt file
 ******************************/

#define char unsigned char

typedef struct { 
   char data[16]; 
   char state[48]; 
   char checksum[16]; 
 } MD2; 

static char PI_SUBST[256] = {
  41, 46, 67, 201, 162, 216, 124, 1, 61, 54, 84, 161, 236, 240, 6,
  19, 98, 167, 5, 243, 192, 199, 115, 140, 152, 147, 43, 217, 188,
  76, 130, 202, 30, 155, 87, 60, 253, 212, 224, 22, 103, 66, 111, 24,
  138, 23, 229, 18, 190, 78, 196, 214, 218, 158, 222, 73, 160, 251,
  245, 142, 187, 47, 238, 122, 169, 104, 121, 145, 21, 178, 7, 63,
  148, 194, 16, 137, 11, 34, 95, 33, 128, 127, 93, 154, 90, 144, 50,
  39, 53, 62, 204, 231, 191, 247, 151, 3, 255, 25, 48, 179, 72, 165,
  181, 209, 215, 94, 146, 42, 172, 86, 170, 198, 79, 184, 56, 210,
  150, 164, 125, 182, 118, 252, 107, 226, 156, 116, 4, 241, 69, 157,
  112, 89, 100, 113, 135, 32, 134, 91, 207, 101, 230, 45, 168, 2, 27,
  96, 37, 173, 174, 176, 185, 246, 28, 70, 97, 105, 52, 64, 126, 15,
  85, 71, 163, 35, 221, 81, 175, 58, 195, 92, 249, 206, 186, 197,
  234, 38, 44, 83, 13, 110, 133, 40, 132, 9, 211, 223, 205, 244, 65,
  129, 77, 82, 106, 220, 55, 200, 108, 193, 171, 250, 36, 225, 123,
  8, 12, 189, 177, 74, 120, 136, 149, 139, 227, 99, 232, 109, 233,
  203, 213, 254, 59, 0, 29, 57, 242, 239, 183, 14, 102, 88, 208, 228,
  166, 119, 114, 248, 235, 117, 75, 10, 49, 68, 80, 180, 143, 237,
  31, 26, 219, 153, 141, 51, 159, 17, 131, 20
};

void calc_md2(MD2 *ctx){
  int count = 0;
  int temp = 0;
  int k = 0;

  for(count = 0; count < 16; count++){
    ctx->state[count+16] = ctx->data[count];
    ctx->state[count+32] = (ctx->state[count+16])^(ctx->state[count]);
  }
  
  temp = 0;
  for(count = 0 ; count < 18; count++){
    for(k = 0; k<48; k++){
      ctx->state[k] ^= PI_SUBST[temp];
      temp = ctx->state[k];
    }
    temp = (temp+count) % 256;
  }
  
  //compute the checksum
  temp = ctx->checksum[15];
  for(count = 0 ; count < 16 ;count++){
    ctx->checksum[count] = PI_SUBST[ctx->data[count]^temp];
    temp = ctx->checksum[count];
  }

}

//char to hex
void ctoh(char data[], char *string){
  printf("%s = ", string);
  char k;
  int count = 0;

  for(count = 0; count < 16; count++){
    k = data[count];
    printf("%X ", k & 0xff);
  }
  printf("\n");

}

int main(int argc, char **argv){
  if(argc < 2){
    printf("ERROR: Not enough parameters \nUSAGE: %s <file_name>\n", argv[0]);
    exit(0);
  }
  char *fileName = argv[1];
  char buffer[16];
  int offset = 0;
  int c; //for getc()
  int i=0;
  int counter = 0;
  MD2 *ctx = malloc(sizeof(MD2));

  memset(ctx->data,0,15);
  for(i = 0; i<48;i++) ctx->state[i] = 0;
  for(i = 0; i<16;i++) ctx->checksum[i] = 0;

  printf("Using File: %s\n", fileName);
  FILE *IF = fopen(fileName, "rb");

  //read one char at a time
  while(( c=getc(IF) ) != EOF){  
    ctx->data[counter] = (char)c;
    counter++;
    if(counter == 16){ //once you get 16 chars calculate md2 and checksum for it
      calc_md2(ctx);
      counter = 0;
    }
  }

  //check for errors
  if(ferror(IF) > 0){
    printf("Error: exiting"); exit(0);
  }

  offset = 16 - counter;

  printf("\npadding with %d bytes \n", offset);

  //pad the data
  while(counter<16){
    ctx->data[counter++] = *((char*)&offset);
  }

  //recalculate md2 with padding
  calc_md2(ctx);
  
  //append checksum
  memcpy(ctx->data, ctx->checksum, 16);

  //recalculate md2 with checksum appended
  calc_md2(ctx);

  printf("\n");

  //print final result
  ctoh(ctx->state, "MD2");

  printf("\n");

  free(ctx);
}

