#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "transform.h"

//void write_mat(FILE *fp, float **testRes, int size);

void write_dct(FILE *file, float **m, int size)
{
	int i, j;
	
	for(i = 0; i < size; i++){
		
		fprintf(file, "%f", m[i][0]);
		
		for(j = 1; j < size; j++){
			
			fprintf(file, "\t%f", m[i][j]);
        	}
		   
		fprintf(file, "\n");
	}
	
	fprintf(file, "\n");
	
}


int main() 
{
	

    float testBlockA[8][8] = {
	    { 127, 127, 127, 127, 127, 127, 127, 127 },
	    { 127, 127, 127, 127, 127, 127, 127, 127 },
	    { 127, 127, 127, 127, 127, 127, 127, 127 },
	    { 127, 127, 127, 127, 127, 127, 127, 127 },
	    { 127, 127, 127, 127, 127, 127, 127, 127 },
	    { 127, 127, 127, 127, 127, 127, 127, 127 },
	    { 127, 127, 127, 127, 127, 127, 127, 127 },
	    { 127, 127, 127, 127, 127, 127, 127, 127 }
    };

    float testBlockB[8][8] = { 
        { 255, 0, 255, 0, 255, 0, 255, 0 },
		{ 0, 255, 0, 255, 0, 255, 0, 255 },
		{ 255, 0, 255, 0, 255, 0, 255, 0 },
		{ 0, 255, 0, 255, 0, 255, 0, 255 },
		{ 255, 0, 255, 0, 255, 0, 255, 0 },
		{ 0, 255, 0, 255, 0, 255, 0, 255 },
		{ 255, 0, 255, 0, 255, 0, 255, 0 },
		{ 0, 255, 0, 255, 0, 255, 0, 255 }
    };
	   

	   FILE * file = fopen("dct_out.csv", "w");
	   int size = 8, i, j;
	   
	   float **testDCT = malloc2d(size);
	   float **testiDCT = malloc2d(size);
	   float **blockA = malloc2d(size);
       float **blockB = malloc2d(size);
       float **temp1 = malloc2d(size);
       float **temp2 = malloc2d(size);

       for(i = 0; i < size; i++){
           for(j = 0; j < size; j++){
               blockA[i][j] = testBlockA[i][j];
               blockB[i][j] = testBlockB[i][j];
           }
       }

       fprintf(file, "Original Block A\n");
       write_dct(file, blockA, size);

	   fprintf(file, "DCT Transformed Block A\n");
       dct(testDCT, blockA, size);
	   write_dct(file, testDCT, size);
	   
       fprintf(file, "quantize with Q= 20\n");
       quantize(temp1, blockA, size, 20);
       dequantize(temp2, temp1, size, 20);
       idct(testiDCT, temp2, size);
       write_dct(file, testiDCT, size);
       

       fprintf(file, "quantize with Q= 50\n");
       quantize(temp1, blockA, size, 50);
       dequantize(temp2, temp1, size, 50);
       idct(testiDCT, temp2, size);
       write_dct(file, testiDCT, size);


       fprintf(file, "quantize with Q= 80\n");
       quantize(temp1, blockA, size, 80);
       dequantize(temp2, temp1, size, 80);
       idct(testiDCT, temp2, size);
       write_dct(file, testiDCT, size);
       
       fprintf(file, "iDCT Transformed Block A\n");
	   idct(testiDCT, testDCT, size);
	   write_dct(file, testiDCT, size);


       fprintf(file, "Original Block\n");
       write_dct(file, blockB, size);

       fprintf(file, "DCT Transformed Block B\n");
       dct(testDCT,blockB, size);
       write_dct(file, testDCT, size);
          
       fprintf(file, "iDCT Transformed Block B\n");
       idct(testiDCT, testDCT, size);
       write_dct(file, testiDCT, size);



	   fclose(file);
	   free_mat2d(testDCT, 8);
	   free_mat2d(testiDCT, 8);
       free_mat2d(blockA, 8);
       free_mat2d(blockB, 8);
	   
	   return 0;
	   
}
