#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ee554.h"    /* EE554 software library           */
 
#include "transform.h"
     
void do_dct(image *img1, image *img2, int Q);

//calc_PSNR_image(image *img1, image *img2);

int main(int argc, char *argv[])
{
    image *img_in,       /* pointer to input image data structure  */
         *img_out;      /* pointer to output image data structure */
 
    char  file_in[100],  /* input file name */
         file_out[100]; /* output file name */
    double snr_value = 0.0;

    /* Process input arguments */
    if(argc != 4)
    {
        fprintf(stdout,"Usage: example [input PGM file name] [output PGM file name] [\"Q\" value] \n");
        exit(0);
    }
    else
    {
        strcpy(file_in, argv[1]);
        strcpy(file_out, argv[2]);
    }
 
    /* Load the input image */
    img_in = pgm_load_image(file_in);
 
    /* Make a clone of it to store the result */
    img_out = clone_image(img_in);

    /* do DCT, iDCT, quantization and unqantization */
    do_dct(img_in, img_out, atoi(argv[3]));
 
    /* when done calculate the PSNR of the original and processed image */
    snr_value = calc_PSNR_image(img_in, img_out);
    printf("The value of PSNR for %s with Q = %s is: %f\n", file_out, argv[3], snr_value);

    printf("The value of PSNR for same picture is: %f\n", calc_PSNR_image(img_in, img_in));
    /* Now write out the output image */
    printf("Writing out %s\n",file_out);
    pgm_write_image(img_out, file_out);
 
   
    free_image(img_in);
    free_image(img_out);
 
    return(EXIT_SUCCESS);
}

void do_dct(image *img1, image *img2, int Q)
{
   block_t blk;         /* macroblock to be processed */
   int   width,         /* width of image */
         height,        /* height of image */
         x, y,          /* macroblock loop indexes */
         i, j;          /* pixel loop indexes */

   /* Get image width and height */
   width = get_image_width(img1);
   height = get_image_height(img1);

   /* Scan through the image and process macroblocks */
   for(y = 0; y < height; y+=8) {  /* block width is 8 pixels */
     for(x = 0; x < width; x+=8) { /* block height is 8 pixels */

      /* get a macroblock from the input image */
       get_block(img1, x, y, blk);
 
       float **dct_blk = malloc2d(8);
       float **idct_blk = malloc2d(8);
       float **in_blk = malloc2d(8);
       float **qout = malloc2d(8);
       float **deqout = malloc2d(8);

       for(i = 0; i < 8; i++){
           for(j = 0; j < 8; j++){
               in_blk[i][j] = blk[i][j];
           }
       }

       dct(dct_blk, in_blk, 8);
       quantize(qout, dct_blk, 8, Q);
       dequantize(deqout, qout, 8, Q);
       idct(idct_blk, deqout, 8);

       for(i = 0; i < 8; i++){
           for(j = 0; j < 8; j++){
               blk[i][j] = idct_blk[i][j];
           }
       }
 
        free_mat2d(dct_blk, 8);
        free_mat2d(idct_blk, 8);
        free_mat2d(in_blk, 8);
        free_mat2d(qout, 8);
        free_mat2d(deqout, 8);
       /* put the macroblock in the output image */
        put_block(img2, x, y, blk);
     }
   }
   return;
}
