/***********************************************************HeaderBegin*******
 *                                                                         
 * File:        example2.c
 *
 * Author:      Vivien Chappelier
 *
 * Created:     13/07/2001
 *                                                                         
 * Description: Source file for EE554 software library
 *
 * Modified:   
 *              
 ***********************************************************HeaderEnd*********/

/************************    include files    ********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ee554.h"    /* EE554 software library           */
#include "example2.h" /* Header file for this source file */

/***********************************************************CommentBegin******
 *
 * -- main -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            13/07/2001
 *	
 *
 * Description :        Example of using the ee554 software library
 *		
 * 
 * Arguments in :       Two command line arguments: 
 *                      input PGM sequence name
 *                      output PGM sequence name 
 *	                
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :      EXIT_SUCCESS or 1 if the input sequence is too short
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

int main(int argc, char *argv[])
{
  sequence *seq;       /* pointer to input sequence data structure  */
  image    **imgs;     /* pointer to input sequence array of images */
  image    *out;       /* pointer the output image */

  char  seq_in[100],  /* input sequence name */
        seq_out[100], /* output sequence name */
        file_out[100]; /* output file name */

  int length;         /* length of the sequence */
  int i;              /* loop index */

  /* Process input arguments */
  if(argc != 3)
    {
      fprintf(stderr,"Usage: %s [input PGM sequence name] [output PGM sequence name]\n", argv[0]);
      exit(0);
    }
  else
    {
      strcpy(seq_in, argv[1]);
      strcpy(seq_out, argv[2]);
    }

  /* Load the input sequence */
  seq = load_sequence(seq_in);

  /* Get a pointer to the sequence images */
  imgs = get_sequence_images(seq);

  /* Get sequence length */
  length = get_sequence_length(seq);

  /* Check if sequence is long enough */
  if(length < 2) {
    fprintf(stderr, "sequence is too short\n");
    exit(1);
  }

  /* Clone the first image to store results */
  out = clone_image(imgs[0]);

  /* Loop through the sequence and write the mean sequence       */
  /* i.e. for each image of the input sequence, write the image  */
  /* resulting from the mean between the current image and       */
  /* the next one.                                               */

  for(i = 0; i < length - 1; i++) {
    /* Compute the mean between the current picture and the next one */
    mean_image(imgs[i], imgs[i+1], out);

    /* Now write out the output image */
    sprintf(file_out, "%s%03d.pgm", seq_out, i);
    printf("Writing out %s\n",file_out);
    pgm_write_image(out, file_out);
  }

  /* We must deallocate the memory the sequence and for the image before */
  /* we finish */
  free_image(out);
  free_sequence(seq);

  return(EXIT_SUCCESS);
}

/***********************************************************CommentBegin******
 *
 * -- mean_image -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            13/07/2001
 *	
 *
 * Description :        Interpolates two images. The result is stored in
 *                      another image.
 *		
 * 
 * Arguments in :       image *img1 - pointer to first image data structure
 *                      image *img2 - pointer to second image data structure
 *                      image *out - pointer to output image data structure
 *	                
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :      None
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           main()
 *	
 *
 * Modified :           
 *
 *
 ***********************************************************CommentEnd********/

void mean_image(image *img1, image *img2, image *out)
{
  mblock_t b1, b2;      /* macrobblocks to be averaged */ 
  int   width,          /* width of image */
        height,         /* height of image */ 
        x, y,           /* block loop indexes */
        i, j;           /* pixel loop indexes */

  /* Get image width and height */
  width = get_image_width(img1);
  height = get_image_height(img1);

  /* Scan through the images and process blocks */
  for(y = 0; y < height; y+=16) {  /* block width is 16 pixels */
    for(x = 0; x < width; x+=16) { /* block height is 16 pixels */

      /* get a block from the first input image */
      get_mblock(img1, x, y, b1);

      /* get a block from the second input image */
      get_mblock(img2, x, y, b2);

      /* Scan through the blocks and interpolate */
      /* i.e. pixel_out = (pixel_in + pixel_out) / 2 */
      for(i = 0; i < 16; i++) {
	for(j = 0; j < 16; j++) {

	  /* store the result in block 1 */
	  b1[i][j] = (b1[i][j] + b2[i][j]) / 2;
	}
      }

      /* put the block in the output image */
      put_mblock(out, x, y, b1);
    }
  }
  return;
}
