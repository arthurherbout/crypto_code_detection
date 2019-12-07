/***********************************************************HeaderBegin*******
 *                                                                         
 * File:        ee554_lib.c
 *
 * Author:      Noel O'Connor
 *
 * Created:     02/08/2000
 *                                                                         
 * Description: Source file for EE554 software library
 *
 * Modified:    13/07/2001 - Vivien Chappelier: split into private/public versions
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
 * -- get_image_pels -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Returns a pointer to the pixel (pel) data stored in 
 *                      an image data structure.
 *
 *                      *** n.b. ***
 *                      This function should be used to access an image
 *                      data structure rather than the usual 
 *                      pointer operator: img->pels
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
 * Return values :	A pointer to an array of pels
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

pel *get_image_pels(image *img)
{
  return(img->pels);
}

/***********************************************************CommentBegin******
 *
 * -- put_image_width -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Stores the value of the width passed in the
 *                      corresponding element of an image data structure.
 * 
 *                      *** n.b. ***
 *                      These functions should be used to access an image
 *                      data structure rather than the usual 
 *                      pointer operator: img->width
 *		
 * 
 * Arguments in : 	image *img - pointer to the image data structure
 *                      int width  - width of the image
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
 * See also :           get_image_width()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

void put_image_width(image *img, int width)
{
  img->width = width;
}

/***********************************************************CommentBegin******
 *
 * -- put_image_height -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Stores the value of the height passed in the
 *                      corresponding element of an image data structure.
 * 
 *                      *** n.b. ***
 *                      These functions should be used to access an image
 *                      data structure rather than the usual 
 *                      pointer operator: img->width
 *		
 * 
 * Arguments in : 	image *img - pointer to the image data structure
 *                      int width  - width of the image
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
 * See also :           get_image_height()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

void put_image_height(image *img, int height)
{
  img->height = height;
}

/***********************************************************CommentBegin******
 *
 * -- pgm_read_image -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Reads a PGM format image on disk into an image data
 *                      structure. The PGM image on disk must be of type
 *                      P5 (i.e. byte PGM). Any existing pixel data in the
 *                      image structure will be overwritten. 
 *
 *                      *** n.b. ***
 *                      (1) It is assumed that memory for the image data 
 *                          structure is already allocated!
 *                      (2) The size of the image in memory and the size 
 *                          of the image on disk must be equal!
 *                      
 *                      Note: Allocates a temporary byte buffer, reads 
 *                      into buffer, and then copies buffer to pels. This 
 *                      effectively decouples pel data type and PGM format.
 *                      As a consequence, currently only 8-bit PGM byte 
 *                      format is supported.
 *		
 *
 * 
 * Arguments in : 	char *file_name - file name of image file on disk
 *                      image *img      - pointer to image data structure 
 *                                        to be filled
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
 * See also :           pgm_read_image_header(), pgm_read_image_header_bytes(),
 *                      pgm_write_image()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

void pgm_read_image(char *file_name, image *img)
{
  FILE *fpfile;
  
  int i, width, height, size, max_gl;

  pel *pdat;

  unsigned char *buffer;

  /* Open the file for reading */
  if((fpfile = fopen(file_name,"rb")) == NULL)
    {
      fprintf(stderr,"Error in pgm_read_image(): could not open file %s!\n",file_name);
      exit(1);
    }

  pgm_read_image_header_bytes(fpfile,&width, &height, &max_gl);

  /* Error checking */
  if((width != get_image_width(img)) || (height != get_image_height(img)))
    {
      fprintf(stderr,"Error in pgm_read_image(): image structure and image to be read must have the same dimensions!\n");
      exit(1);
    }

  size = width * height;

  /* allocate temporary (byte) buffer */
  if((buffer = (unsigned char *)malloc(size * sizeof(unsigned char))) == NULL)
    {
      fprintf(stderr,"Error in pgm_read_image(): buffer memory allocation failed!\n");
      exit(1);
    }

  /* Read pel data into temporary buffer */
  if((fread(buffer, sizeof(unsigned char), size, fpfile)) != size)
    {
      fprintf(stderr,"Error in pgm_read_image(): read failure on image %s!\n",file_name);
      exit(1);
      }

  /* copy image pixel data from temporary (byte) buffer */
  pdat = get_image_pels(img);
  for(i = 0; i < size; i++, pdat++)
    *pdat = (pel)buffer[i];

  /* free temporary buffer */
  free(buffer);

  /* Close the file */
  fclose(fpfile);

  return;
}

/***********************************************************CommentBegin******
 *
 * -- pgm_read_image_header -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Reads the header information of a PGM format image
 *                      file on disk. Useful for finding out the size of 
 *                      an image on disk prior to allocating memory for
 *                      it.
 *		
 * 
 * Arguments in : 	char *file_name - file name of PGM image on disk
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	int *width  - width of image on disk
 *                      int *height - height of image on disk
 *                      int *max_gl - maximum grey level in image on disk
 *	
 *
 * Return values :	None
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           pgm_read_image(), pgm_read_image_header_bytes(),
 *                      pgm_write_image() 
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

void pgm_read_image_header(char *file_name, int *width, int *height, int *max_gl)
{
  FILE *fpfile;

  /* Open the file for reading */
  if((fpfile = fopen(file_name,"rb")) == NULL)
    {
      fprintf(stderr,"Error in pgm_read_image_header(): could not open file %s!\n",file_name);
      exit(1);
    }

  /* Read the header byte by byte */
  pgm_read_image_header_bytes(fpfile, width, height, max_gl);

  /* Close the file */
  fclose(fpfile);

  return;
  
}

/***********************************************************CommentBegin******
 *
 * -- pgm_read_image_header_bytes -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *
 *	
 * Description :	Reads the header information of a PGM format image
 *                      file on disk from a file pointer which points to the
 *                      ***already opened*** file. 
 *
 *		
 * 
 * Arguments in : 	FILE *fpfile - file pointer to *opened* image file
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	int *width  - width of image on disk
 *                      int *height - height of image on disk
 *                      int *max_gl - maximum grey level in image on disk
 *	
 *
 * Return values :	None
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           pgm_read_image(), pgm_read_image_header(),
 *                      pgm_write_image() 
 *	
 *
 * Modified :           13/07/2001 - Vivien Chappelier: fixed bug in parser
 *
 *
 ***********************************************************CommentEnd********/

void pgm_read_image_header_bytes(FILE *fpfile,int *width, int *height, int *max_gl)
{
  char magic1, magic2, byte, comment[70];

  int value[3] = {0,0,0}, count =0;

  /* check we are dealing with a byte pgm file (Ascii pgm not supported) */
  fread(&magic1,sizeof(char),1,fpfile);
  fread(&magic2,sizeof(char),1,fpfile);
  if((magic1 != 'P') || (magic2 != '5'))
    {
      fprintf(stderr,"Error in pgm_read_image_header_bytes(): file is not a byte format pgm file!");
      exit(1);
    }

  while(count < 3)
    {
      /* read bytes, check for comments and valid numbers */
      fread(&byte,sizeof(char),1,fpfile);

      switch (byte)
	{
	case '#' : fgets(comment,70,fpfile); break; 
	case ' ' : break;
	case '\n': break;
	case '\r': break;
	default  : fseek(fpfile, -1, SEEK_CUR);
	           if(count < 3)
		       fscanf(fpfile, "%d", &value[count]); 
		   count++;
		   break;
	}
    }
  /* Need to skip until valid image data */
  byte = fread(&byte,sizeof(char),1,fpfile);
  switch (byte)
  {
    case '#' : fgets(comment,70,fpfile); break; 
    case ' ' : break;
    case '\n': break;
    case '\r': break;
    default  : break;
  }

  *width = value[0];
  *height = value[1];
  *max_gl = value[2];

  return;
}

/***********************************************************CommentBegin******
 *
 * -- calc_MSE_image -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Calculates the Mean Square Error (MSE) between two
 *                      input images
 *		
 * 
 * Arguments in : 	image *img1 - pointer to the first image
 *                      image *img2 - pointer to the second image
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	The MSE as a double value
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           calc_PSNR_image()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

double calc_MSE_image(image *img1, image *img2)
{
  double mse = 0.0;

  int x,y,w1,h1,w2,h2;

  pel *pdat1, *pdat2;

  w1 = get_image_width(img1);
  h1 = get_image_height(img1);
  w2 = get_image_width(img2);
  h2 = get_image_height(img2);

  /* Error checking */
  if((w1 != w2) || (h1 != h2))
    {
      fprintf(stderr,"Error in calc_mse_image(): images must be the same dimensions!\n");
      exit(1);
    }

  pdat1 = get_image_pels(img1);
  pdat2 = get_image_pels(img2);

  for(y = 0; y < h1; y++)
    for(x = 0; x < w1; x++, pdat1++, pdat2++)
      mse += ((double) *pdat1 - (double) *pdat2) * ((double) *pdat1 - (double) *pdat2);

  mse = mse/(h1 * w1);

  return(mse);  
}

/***********************************************************CommentBegin******
 *
 * -- estimate_bits_image -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Estimates (very roughly!) the number of bits 
 *                      required to code an image
 *		
 * 
 * Arguments in : 	image *img - pointer to the image
 *                      int        - indicates whether -ve pel values 
 *                                   are present 
 *                                   (1 -> -ve and +ve pel values)
 *                                   (0 -> only +ve pel values)   
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	The estimated number of bits
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           estimate_bits_pel() and estimate_bits_pel_signed()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

int estimate_bits_image(image *img, int sign)
{
  int width, height, x, y, p = 0, bits = 0;

  pel *pdata;

  width = get_image_width(img);
  height = get_image_height(img);

  pdata = get_image_pels(img);
  
  for(y = 0; y < height; y++)
    for(x = 0; x < width; x++, p++)
      {
	if(sign == 1)
	  bits += estimate_bits_pel_signed(pdata[p]);
	else
	  bits += estimate_bits_pel(pdata[p]);
      }
  return(bits);
}

/***********************************************************CommentBegin******
 *
 * -- estimate_bits_pel -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Estimates (very roughly!) the number of bits 
 *                      required to code a +ve pel value
 *		
 * 
 * Arguments in : 	pel pval - the pel value
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	The estimated number of bits
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           estimate_bits_image() and estimate_bits_pel_signed()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

int estimate_bits_pel(pel p_val)
{
  double db_val, db_bits;

  int bits;

  db_val = (double)p_val;

  /* Handle pel val of zero */
  if(p_val == 0.0)
    bits = 1;
  else
    {
      db_bits = log2(fabs(db_val));
      /* need to round up i.e. truncate plus one  */
      bits = (int)db_bits + 1;
    }

  return(bits);
}

/***********************************************************CommentBegin******
 *
 * -- estimate_bits_pel_signed -- 
 *
 * Author :             Noel O'Connor	
 *	
 *
 * Created :            02/08/2000
 *	
 *
 * Description :	Estimates (very roughly!) the number of bits 
 *                      required to code a pel value where the value 
 *                      could be -ve or +ve (e.g. a difference image)
 *		
 * 
 * Arguments in : 	pel pval - the pel value
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	None
 *	
 *
 * Return values :	The estimated number of bits
 *
 *
 * Side effects :       None
 *	
 *
 * See also :           estimate_bits_image() and estimate_bits_pel()
 *	
 *
 * Modified :  
 *
 *
 ***********************************************************CommentEnd********/

int estimate_bits_pel_signed(pel p_val)
{
  double db_val, db_bits;

  int bits;

  db_val = (double)p_val;

  /* Handle pel val of zero */
  if(p_val == 0.0)
    bits = 1;
  else
    {
      db_bits = log2(fabs(db_val));
      /* need to round up and account for sign */
      /* truncate plus one (round) plus one (sign) */
      bits = (int)db_bits + 2;
    }

  return(bits);
}

/***********************************************************CommentBegin******
 *
 * -- copy_image -- 
 *
 * Author : Noel O'Connor	
 *	
 *
 * Created : 02/08/2000
 *	
 *
 * Description :	Copies the pixel data of one image to another
 *                      (thereby overwriting its contents). Assumes
 *                      *both* images are already allocated.
 *		
 * 
 * Arguments in : 	image *in - a pointer to the image to be copied
 *	
 *
 * Arguments in/out :	None
 *	
 *
 * Arguments out :	image *out - where to copy the input image
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

void copy_image(image *in, image *out)
{
  int x,y,w1,h1,w2,h2;

  pel *pdatin, *pdatout;

  w1 = get_image_width(in);
  h1 = get_image_height(in);
  w2 = get_image_width(out);
  h2 = get_image_height(out);

  /* Error checking */
  if((w1 != w2) || (h1 != h2))
    {
      fprintf(stderr,"Error in copy_image(): images must be the same dimensions!\n");
      exit(1);
    }

  pdatin = get_image_pels(in);
  pdatout = get_image_pels(out);

  for(y = 0; y < h1; y++)
    for(x = 0; x < w1; x++, pdatin++, pdatout++)
      *pdatout = *pdatin;

  return;  
}


