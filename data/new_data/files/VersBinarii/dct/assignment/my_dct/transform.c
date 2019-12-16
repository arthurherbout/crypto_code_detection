#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "transform.h"

/*
 * DCT is given by following equation:
 *
 * C(u,v) = A(u) A(v) ∑ ∑ f(x,y) cos[(П(2x+1)u)/2N] cos[(П(2y+1)v)/2N]
 *
 */

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
 

void free_mat2d(float **mat, int size)
{
    int i;

    for(i = 0; i < size; i++){
        free(mat[i]);
    }

    free(mat);
}

static float A(int val){
   if(val == 0){
       return 1.0 / sqrt(2.0);
   }else{
       return 1.0;
   }
}

void dct(float **out_mx, float **in_mx, int size){

    int v, u, x, y;
    float temp;

    for(v = 0; v < size; v++){
        for(u = 0; u < size; u++){
            
            out_mx[v][u] = 0;
            temp = 0;

            for(x = 0; x < size; x++){
                for(y = 0; y < size; y++){
            
                    temp += in_mx[x][y] * cos(((2*x+1) * v * M_PI) / (2 * size)) * cos(((2*y+1) * u * M_PI) / (2 * size));
                    //printf("temp is: %f\n", temp);
                }
            }
            
            out_mx[v][u] = temp * A(u) * A(v) * 0.25;
        }
    }
}


void idct(float **out_mx, float **in_mx, int size){
    
    int v, u, x, y;
    float temp;

    for(x = 0; x < size; x++){
        for(y = 0; y < size; y++){
            out_mx[x][y] = 0;
            temp = 0;

            for(u = 0; u < size; u++){
                for(v = 0; v < size; v++){
                    
                    temp += in_mx[v][u] * A(u) * A(v) * cos(((2*x+1) * v * M_PI) / (2 * size)) 
                                                           * cos(((2*y+1) * u * M_PI) / (2 * size));
                }
            }

            out_mx[x][y] = temp * 0.25;
        }
    }
}

void quantize(float **fq_mx, float **f_mx, int size, int Q){

    int i,j;

    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            fq_mx[i][j] = 0.0;

            fq_mx[i][j] = (int)f_mx[i][j] / Q;
        }
    }
}

void dequantize(float **f_hat_mx, float **fq_mx, int size, int Q){

    int i, j;

    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            f_hat_mx[i][j] = 0.0;

            f_hat_mx[i][j] = (int)fq_mx[i][j] * Q;
        }
    }
}
