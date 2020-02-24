/***********************************************************HeaderBegin*******
 *                                                                         
 * File:        ee554.c
 *
 * Author:      Noel O'Connor, Vivien Chappelier
 *
 * Created:     02/08/2000
 *                                                                         
 * Description: Source file for EE554 software library
 *
 * Modified:    13/07/2001 - Vivien Chappelier: 
 *                split ee554.c into separate private/public parts
 *                added sequence, block, macroblock and image loading functions
 *
 ***********************************************************HeaderEnd*********/


/************************    include files    ********************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ee554.h"
#include "ee554_lib.h"


/***********************************************************CommentBegin******
 *
 * -- alloc_image -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :        Allocates memory for an image data structure
 *		
 * 
 * Arguments in :       int width  - width of image in pixels
 *	                int height - height of image in pixels
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :      pointer to the allocated image structure	
 *
 *
 * Side effects :       Sets the width and height elements of image
 *                      data structure
 *	
 *
 * See also :           free_image(), clone_image()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

image *alloc_image(int width, int height)
{
  image *img;

  img = (image *)malloc(sizeof(image));

  put_image_width(img, width);
  put_image_height(img, height);

  /* Error checking */
  if((img->pels = (pel *)calloc(width * height,sizeof(pel))) == NULL)
    {
      fprintf(stderr,"Error in alloc_image(): memory allocation failed!\n");
      exit(1);
    }

  return(img);
}

/***********************************************************CommentBegin******
 *
 * -- clone_image -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            12/07/2001
 *	
 *
 * Description :	Creates another instance of the image.
 *		
 * 
 * Arguments in : 	image *img - a pointer to the image to be cloned
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	A pointer to the clone of the original image
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           alloc_image(), free_image()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

image *clone_image(image * img) {
  image * clone;

  clone = alloc_image(img->width, img->height);
  copy_image(img, clone);
  return(clone);
}

/***********************************************************CommentBegin******
 *
 * -- free_image -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Deallocates the memory associated with an image 
 *                      data structure
 *		
 * 
 * Arguments in : 	image *img - a pointer to the image to be deallocated
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	None
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           alloc_image(), clone_image()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

void free_image(image *img)
{
  free(img->pels);
  free(img);
}

/***********************************************************CommentBegin******
 *
 * -- get_image_width -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Returns the value of the width element of 
 *                      an image data structure.
 * 
 *                      *** n.b. ***
 *                      These functions should be used to access an image
 *                      data structure rather than the usual 
 *                      pointer operator: img->width
 *		
 * 
 * Arguments in : 	image *img - a pointer to the image data structure
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	The image width as an integer (int) value
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

int get_image_width(image *img)
{
  return(img->width);
}

/***********************************************************CommentBegin******
 *
 * -- get_image_height -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Returns the value of the height element of 
 *                      an image data structure.
 * 
 *                      *** n.b. ***
 *                      These functions should be used to access an image
 *                      data structure rather than the usual 
 *                      pointer operator: img->height.
 *		
 * 
 * Arguments in : 	image *img - a pointer to the image data structure
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	The image height as an integer (int) value
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

int get_image_height(image *img)
{
  return(img->height);
}

/***********************************************************CommentBegin******
 *
 * -- pgm_load_image -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            12/07/2001
 *	
 *
 * Description :	Loads an image from a file.
 *		
 * 
 * Arguments in : 	char *filename - name of the image file to be loaded
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	A pointer to the image
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           pgm_write_image(), free_image()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

image *pgm_load_image(char *filename) {
  int width, height, max_gl;
  image *img;

  pgm_read_image_header(filename, &width, &height, &max_gl);
  img = alloc_image(width, height);
  pgm_read_image(filename, img);

  return(img);
}

/***********************************************************CommentBegin******
 *
 * -- pgm_write_image -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Writes the contents of an image data structure to
 *                      disk as a PGM file (of type P5).
 *
 *                      Note: Allocates a temporary byte buffer and copies
 *                      pels to buffer, before buffer is written. This 
 *                      effectively decouples pel data type and PGM format.
 *                      As a consequence, currently only 8-bit PGM byte 
 *                      format is supported.
 *		
 * 
 * Arguments in : 	image *img     - pointer to image data structure to be 
 *                                       written
 *                      char *filename - file name of PGM format image file
 *                                       on disk
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	None
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           pgm_load_image()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

void pgm_write_image(image *img, char *file_name)
{
  FILE *fpfile;

  int  width = get_image_width(img), 
       height = get_image_height(img),
       size = width * height,
       i;

  pel  *pdat;

  unsigned char *buffer;

  /* Open the file for writing */
  if((fpfile = fopen(file_name,"wb")) == NULL)
    {
      fprintf(stderr,"Error in pgm_write_image(): could not open file %s!\n",file_name);
      exit(1);
    }

  /* Write PGM Header */
  fprintf(fpfile,"P5 %d %d %d\n", width, height, 255);

  /* allocate temporary (byte) buffer */
  if((buffer = (unsigned char *)malloc(size * sizeof(unsigned char))) == NULL)
    {
      fprintf(stderr,"Error in pgm_write_image(): buffer memory allocation failed!\n");
      exit(1);
    }
  
  /* copy image pixel data from temporary (byte) buffer */
  pdat = get_image_pels(img);
  for(i = 0; i < size; i++, pdat++)
    buffer[i] = (unsigned char)*pdat;

  /* Write pel data */
  if((fwrite(buffer, sizeof(unsigned char), width*height, fpfile)) != width *height)
    {
      fprintf(stderr,"Error in pgm_write_image(): write failure on image %s!\n",file_name);
      exit(1);
    }

  /* free temporary buffer */
  free(buffer);

  /* Close the file */
  fclose(fpfile);

  return;
}


/***********************************************************CommentBegin******
 *
 * -- load_sequence -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            12/07/2001
 *	
 *
 * Description :	Loads a sequence of images from a set of PGM image 
 *                      files. The filename of the image set should be the
 *                      name of the sequence followed by a 3-digit number
 *                      indicating the order of the image sequence. For
 *                      example: mad000.pgm, mad001.pgm, ...
 *		
 * 
 * Arguments in :       char * seqname - sequence name (e.g. "mad")
 *
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	A pointer to the sequence of images
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           free_sequence()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

sequence * load_sequence(char *seqname) {
  char filename[256];
  int i;
  FILE *fpfile;
  int width, height, max_gl;
  sequence *seq;
  image *img;

  seq = (sequence *) malloc(sizeof(sequence));

  for(i = 0; i < 1000; i++) {
    sprintf(filename, "%s%03d.pgm", seqname, i);

    if((fpfile = fopen(filename,"rb")) == NULL) break;

    pgm_read_image_header_bytes(fpfile, &width, &height, &max_gl);
    fclose(fpfile);
    img = alloc_image(width, height);
    pgm_read_image(filename, img);

    seq->images[i] = img;
  }

  seq->length = i;

  return(seq);
}


/***********************************************************CommentBegin******
 *
 * -- free_sequence -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            12/07/2001
 *	
 *
 * Description :	Deallocates the memory associated with an sequence 
 *                      data structure
 *
 * 
 * Arguments in :       sequence * seq - a pointer to the sequence
 *
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	None
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           load_sequence()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

void free_sequence(sequence *seq) {
  int i;

  for(i = 0; i < seq->length; i++)
    free_image(seq->images[i]);
  free(seq);
}

/***********************************************************CommentBegin******
 *
 * -- get_sequence_width -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            12/07/2001
 *	
 *
 * Description :	Returns the width of the images in the sequence.
 *
 *                      *** n.b. ***
 *                      This function should be used to access a sequence
 *                      data structure rather than the usual 
 *                      pointer operator: seq->images[0]->width
 *
 * 
 * Arguments in :       sequence * seq - a pointer to the sequence
 *
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	The width of images in the sequence.
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           get_sequence_height(),
 *                      get_sequence_length(), get_sequence_images()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

int get_sequence_width(sequence *seq) {
  /* all frames in the sequence are assumed to have the same width */

  if(seq->length == 0) return(0);

  return(seq->images[0]->width);
}

/***********************************************************CommentBegin******
 *
 * -- get_sequence_height -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            12/07/2001
 *	
 *
 * Description :	Returns the height of the images in the sequence.
 *
 *                      *** n.b. ***
 *                      This function should be used to access a sequence
 *                      data structure rather than the usual 
 *                      pointer operator: seq->images[0]->height
 *
 * 
 * Arguments in :       sequence * seq - a pointer to the sequence
 *
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	The height of images in the sequence.
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           get_sequence_width(),
 *                      get_sequence_length(), get_sequence_images()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

int get_sequence_height(sequence *seq) {
  /* all frames in the sequence are assumed to have the same height */

  if(seq->length == 0) return(0);

  return(seq->images[0]->height);
}

/***********************************************************CommentBegin******
 *
 * -- get_sequence_length -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            12/07/2001
 *	
 *
 * Description :	Returns the number of images in the sequence.
 *
 *                      *** n.b. ***
 *                      This function should be used to access a sequence
 *                      data structure rather than the usual 
 *                      pointer operator: seq->length
 *
 * 
 * Arguments in :       sequence * seq - a pointer to the sequence
 *
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	The number of frames contained in the sequence
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           get_sequence_width(), get_sequence_height(),
 *                      get_sequence_images()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

int get_sequence_length(sequence *seq) {
  return(seq->length);
}

/***********************************************************CommentBegin******
 *
 * -- get_sequence_images -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            12/07/2001
 *	
 *
 * Description :	Returns a pointer to the images of the sequence.
 *
 *                      *** n.b. ***
 *                      This function should be used to access a sequence
 *                      data structure rather than the usual 
 *                      pointer operator: seq->images
 *
 * 
 * Arguments in :       sequence * seq - a pointer to the sequence
 *
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	A pointer to the array of images contained in
 *                      the sequence 
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           get_sequence_width(), get_sequence_height(),
 *                      get_sequence_length()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

image **get_sequence_images(sequence *seq) {
  return(seq->images);
}


/***********************************************************CommentBegin******
 *
 * -- calc_PSNR_image -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Calculates the Peak Signal to Noise Ratio (PSNR)
 *                      in dB between two images 
 *		
 * 
 * Arguments in : 	image *img1 - pointer to the first image
 *	                image *img2 - pointer to the second image
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	The PSNR as a double value
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

double calc_PSNR_image(image *img1, image *img2)
{
  double mse, psnr;

  mse = calc_MSE_image(img1, img2);

  psnr = 20 * log10(255.0/sqrt(mse));

  return(psnr);
}

/***********************************************************CommentBegin******
 *
 * -- calc_entropy_image -- 
 *
 * Author : Noel O'Connor	
 *	
 * 
 * Created : 02/08/2000
 *	
 *
 * Description :	Calculates the entropy in bits/symbol of a image
 *		
 * 
 * Arguments in : 	image *img  - a pointer to the image
 *                      int   minus - flag indicating whether or not img is a 
 *                                    difference image (or prediction 
 *                                    residual) or an original image
 *                            1/TRUE/+ve => difference image
 *	                      0/FALSE/-ve => original image
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	double entropy - the entopy in bits/symbol
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
double calc_entropy_image(image *img, int minus)
{
  double *prob, entropy;

  pel *pdat;

  int p;

  pdat = get_image_pels(img);

  if(minus)
    {
      prob = (double *)calloc(512,sizeof(double));

      for(p=0;p<get_image_width(img)*get_image_height(img);p++)
	prob[pdat[p]+256]++;
      
      for(p=0;p<512;p++)
	prob[p] = prob[p]/(get_image_width(img)*get_image_height(img));

      entropy = 0.0;
      for(p=0;p<512;p++)
	if(prob[p] != 0.0)
	  entropy += -prob[p] * log2(prob[p]);
    }
  else
    {
      prob = (double *)calloc(256,sizeof(double));

      for(p=0;p<get_image_width(img)*get_image_height(img);p++)
	prob[pdat[p]]++;
      
      for(p=0;p<256;p++)
	prob[p] = prob[p]/(get_image_width(img)*get_image_height(img));
      
      entropy = 0.0;
      for(p=0;p<256;p++)
	if(prob[p] != 0.0)
	  entropy += -prob[p] * log2(prob[p]);
    }  

 free(prob);

 return(entropy);
}

/***********************************************************CommentBegin******
 *
 * -- sub_image -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Subtracts two images
 *		
 * 
 * Arguments in : 	image *img1 - a pointer to the first image
 *                      image *img2 - a pointer to the second image
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	image *out - a pointer to the output image 
 *                      (stores the difference between
 *                      img1 and img2)  
 *	
 *
 * Return values :	None
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

void sub_image(image *img1, image *img2, image *out)
{
  int x,y,w,h,diff;

  pel *pdat1, *pdat2, *pdatout;

  w = get_image_width(img1);
  h = get_image_height(img1);

  pdat1 = get_image_pels(img1);
  pdat2 = get_image_pels(img2);
  pdatout = get_image_pels(out);

  for(y = 0; y < h; y++)
    for(x = 0; x < w; x++, pdat1++, pdat2++, pdatout++)
      {
	diff = (int)*pdat1 - (int)*pdat2;
	*pdatout = diff;
      }

  return;  
}


/***********************************************************CommentBegin******
 *
 * -- abs_sub_image -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Calculates the absolute difference between two
 *                      images
 *		
 * 
 * Arguments in : 	image *img1 - a pointer to the first image
 *                      image *img2 - a pointer to the second image
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	image *out - a pointer to the output image 
 *                      (stores the absolute difference between
 *                      img1 and img2)  
 *	
 *
 * Return values :	None
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

void abs_sub_image(image *img1, image *img2, image *out)
{
  int x,y,w,h,diff;

  pel *pdat1, *pdat2, *pdatout;

  w = get_image_width(img1);
  h = get_image_height(img1);

  pdat1 = get_image_pels(img1);
  pdat2 = get_image_pels(img2);
  pdatout = get_image_pels(out);

  for(y = 0; y < h; y++)
    for(x = 0; x < w; x++, pdat1++, pdat2++, pdatout++)
      {
	diff = (int)*pdat1 - (int)*pdat2;
	if(diff < 0) diff = -diff;
	*pdatout = (pel)diff;
      }

  return;  
}

/***********************************************************CommentBegin******
 *
 * -- create_diff_image -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Calculates the *difference* image between two 
 *                      images. To form the difference image, the 
 *                      differences between each pixel in the input images 
 *                      is scaled by: (2 * diff) + 128. This produces
 *                      an image which when viewed, clearly illustrates the
 *                      differences.
 *		
 * 
 * Arguments in : 	image *img1 - a pointer to the first image
 *                      image *img2 - a pointer to the second image
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	image *imgout - a pointer to the output difference
 *                                      image
 *	
 *
 * Return values :	None
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

void create_diff_image(image *img1, image *img2, image *imgout)
{
  int x,y,w,h;

  pel *pdat1, *pdat2, *pdatout;

  w = get_image_width(img1);
  h = get_image_height(img1);

  pdatout = get_image_pels(imgout);
  pdat1 = get_image_pels(img1);
  pdat2 = get_image_pels(img2);

  for(y = 0; y < h; y++)
    for(x = 0; x < w; x++, pdat1++, pdat2++, pdatout++)
      *pdatout = (pel)(2 * ((int)*pdat1 - (int)*pdat2) + 128);

  return;
}



/***********************************************************CommentBegin******
 *
 * -- get_block -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            12/07/2001
 *	
 *
 * Description :	Extracts one 8x8 block from an image.
 *		
 * 
 * Arguments in : 	image *img - a pointer to the image
 *                      int x - x coordinate of the upper left corner of
 *                              the block in pixels
 *                      int y - y coordinate of the upper left corner of
 *                              the block in pixels
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	block_t block - the extracted 8x8 block
 *	
 *
 * Return values :	None
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           put_block()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

void get_block(image *img, int x, int y, block_t block)
{
  int i, j;
  pel *p;
  int pitch;

  pitch = img->width;
  p = img->pels + y*pitch + x;

  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
      block[i][j] = p[j];
    }
    p += pitch;
  }
}

/***********************************************************CommentBegin******
 *
 * -- put_block -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            12/07/2001
 *	
 *
 * Description :	Puts an 8x8 block into an image.
 *		
 * 
 * Arguments in :       int x - x coordinate of the upper left corner of
 *                              the block in pixels
 *                      int y - y coordinate of the upper left corner of
 *                              the block in pixels
 *	                block_t block - the 8x8 block to be written in 
 *                                      the image
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	image *img - a pointer to the image
 *	
 *
 * Return values :	None
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           get_block()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

void put_block(image *img, int x, int y, block_t block)
{
  int i, j;
  pel *p;
  int pitch;

  pitch = img->width;
  p = img->pels + y*pitch + x;

  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
      /* rounding */
      p[j] = block[i][j];
      /* clipping */
      if(p[j] < 0) p[j] = 0;
      if(p[j] > 255) p[j] = 255;
    }
    p += pitch;
  }
}

/***********************************************************CommentBegin******
 *
 * -- get_mblock -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            12/07/2001
 *	
 *
 * Description :	Extracts one 16x16 block from an image.
 *		
 * 
 * Arguments in : 	image *img - a pointer to the image
 *                      int x - x coordinate of the upper left corner of
 *                              the macroblock in pixels
 *                      int y - y coordinate of the upper left corner of
 *                              the macroblock in pixels
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	mblock_t mb - the extracted 16x16 block
 *	
 *
 * Return values :	None
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           put_mblock()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

void get_mblock(image *img, int x, int y, mblock_t mb) {
  int i, j;
  pel *p;
  int pitch;

  pitch = img->width;
  p = img->pels + y*pitch + x;

  for(i = 0; i < 16; i++) {
    for(j = 0; j < 16; j++) {
      mb[i][j] = p[j];
    }
    p += pitch;
  }
}

/***********************************************************CommentBegin******
 *
 * -- put_mblock -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            12/07/2001
 *	
 *
 * Description :	Puts an 16x16 block into an image.
 *		
 * 
 * Arguments in :       int x - x coordinate of the upper left corner of
 *                              the macroblock in pixels
 *                      int y - y coordinate of the upper left corner of
 *                              the macroblock in pixels
 *	                mblock_t mb - the 8x8 macroblock to be written in
 *                                    the image
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	image *img - a pointer to the image
 *	
 *
 * Return values :	None
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           get_mblock()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

void put_mblock(image *img, int x, int y, mblock_t mb) {
  int i, j;
  pel *p;
  int pitch;

  pitch = img->width;
  p = img->pels + y*pitch + x;

  for(i = 0; i < 16; i++) {
    for(j = 0; j < 16; j++) {
      p[j] = mb[i][j];
    }
    p += pitch;
  }
}


/***********************************************************CommentBegin******
 *
 * -- print_block -- 
 *
 * Author :             Vivien Chappelier
 *	
 *
 * Created :            12/07/2001
 *	
 *
 * Description :	Prints formated 8x8 block data on the console.
 *		
 * 
 * Arguments in : 	block_t block - block to be printed
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	None
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

void print_block(block_t block) {
  int i, j;

  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
      printf("%.2f ", block[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}
