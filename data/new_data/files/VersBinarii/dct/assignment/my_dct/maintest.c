#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "transform.h"

void write_mat(FILE *fp, float **testRes, int size);
float **malloc2d(int size);
void free2d(float **p);

float **malloc2d(int size){

	int i;
    float **mat = malloc(size * sizeof(float *));

    if(mat != NULL){
        for(i = 0; i < size; i++){
            mat[i] = malloc(size * sizeof(*mat[i]));
            if(mat[i] == NULL){
                return NULL;
            }
        }    
    } else {
        return NULL;
    }
	
	return mat;
}


void free2d(float **m)
{
	free(m[0]);
	free(m);
}


void write_mat(FILE *fp, float **m, int size)
{
	int i, j;
	
	for(i = 0; i < size-1; i++){
		
		fprintf(fp, "%f", m[i][0]);
		
		for(j = 1; j < size-1; j++){
			
			fprintf(fp, "\t%f", m[i][j]);
        	}
		   
		fprintf(fp, "\n");
	}
	
	fprintf(fp, "\n");
	
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
	   

	   FILE * fp = fopen("mydata.csv", "w");
	   int size = 8, i, j;
	   
	   float **testDCT = malloc2d(size);
	   float **testiDCT = malloc2d(size);
	   float **blockA = malloc2d(size);
       float **blockB = malloc2d(size);
       
       for(i = 0; i < size; i++){
           for(j = 0; j < size; j++){
               blockA[i][j] = testBlockA[i][j];
               blockB[i][j] = testBlockB[i][j];
           }
       }

	   dct(testDCT, blockA, size);
	   
	   write_mat(fp, testDCT, size);
	   
	   idct(testiDCT, testDCT, size);
	   
	   write_mat(fp, testiDCT, size);


       dct(testDCT,blockB, size);
            
       write_mat(fp, testDCT, size);
          
       idct(testiDCT, testDCT, size);
           
       write_mat(fp, testiDCT, size);



	   fclose(fp);
	   //free2d(testDCT);
	   //free2d(testiDCT);
       //free2d(blockA);
       //free2d(blockB);
	   
	   return 0;
	   
}
