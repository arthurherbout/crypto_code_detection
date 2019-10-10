#include <stdio.h>
#include <math.h>

int main(int argc, char **argv)
{
    FILE* input = fopen("D:\\Downloads\\Chrome\\A-small-attempt0.in","r"),*output = fopen("D:\\Downloads\\Chrome\\output.txt","w");
    int T,A,B;
    float *p,*r;
    fscanf(input,"%d",&T);
    for(int t=0;t<T;t++){
        fscanf(input,"%d%d",&A,&B);
        p = new float[A];
        int rsize = 1 << A;
        r = new float[rsize];
        for(int i=0;i<rsize;i++){
            r[i] = 1;
        }
        for(int i=0;i<A;i++){
            fscanf(input,"%f",p+i);
            for(int j=0,jend = 1 << (i+1);j<jend;j++){
                for(int k=0,kend = rsize >> (i+1);k<kend;k++){
                    if(j % 2 == 0){
                        r[j*kend+k] *= p[i];
                    }else{
                        r[j*kend+k] *= 1 - p[i];
                    }
                }
            }
        }
        double tryTomatch = (B - A + 1)*r[0],hitEnter = B + 2,*hitBS = new double[A],best;
        for(int i=1;i<rsize;i++){
            tryTomatch += (2*B - A + 2)*r[i];
        }
        best = tryTomatch<hitEnter?tryTomatch:hitEnter;
        for(int i=0;i<A;i++){
            hitBS[i] = 0;
            for(int e=0;e<rsize;e++){
                if((e>>(i+1)) == 0){
                    hitBS[i] += (2*(i+1) + B - A + 1)*r[e];
                }else{
                    hitBS[i] += (2*(i+1) + B - A + 1 + B + 1)*r[e];
                }
            }
            printf("\thitBS[%d] = %f\n",i,hitBS[i]);
            best = best<hitBS[i]?best:hitBS[i];
        }
        fprintf(output,"Case #%d: %f\n",t+1,best);
        /*for(int i=0;i<rsize;i++){
            fprintf(output,"%f ",r[i]);
        }
        fprintf(output,"\n");*/
        delete[] hitBS;
        delete[] p;
        delete[] r;
    }
    fclose(input);
    fclose(output);
    return 0;
}

