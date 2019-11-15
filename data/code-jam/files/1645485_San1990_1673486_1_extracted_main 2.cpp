#include <stdio.h>
#include <math.h>

int main(int argc, char **argv)
{
    FILE* input = fopen("D:\\Downloads\\Chrome\\B-large.in","r"),*output = fopen("D:\\Downloads\\Chrome\\output.in","w");
    int T,A,B;
    float *p;
    fscanf(input,"%d",&T);
    for(int t=0;t<T;t++){
        fscanf(input,"%d%d",&A,&B);
        p = new float[A];
        double allgood = 1, tryTomatch,hitEnter = B + 2,*hitBS = new double[A],best;
        for(int i=0;i<A;i++){
            fscanf(input,"%f",p+i);
            allgood *= p[i];
        }
        tryTomatch = allgood * (B - A + 1) + (1 - allgood) * (2*B - A + 2);
        best = tryTomatch<hitEnter?tryTomatch:hitEnter;
        printf("\ttry to match %f hit enter %f\n",tryTomatch,hitEnter);
        for(int i=0;i<A;i++){
            hitBS[i] = 0;
            double hitGood = 1,hitBad = 1;
            for(int j=0;j<(A-i-1);j++){
                hitGood *= p[j];
            }
            hitBad = 1 - hitGood;
            //printf("\tgood %f bad %f",hitGood,hitBad);
            hitGood *= (2*(i+1) + B - A + 1);
            hitBad *= (2*(i+1) + B - A + 1 + B + 1);
            hitBS[i] = hitGood + hitBad;
            //printf("\thitBS[%d] = %f\n",i,hitBS[i]);
            best = best<hitBS[i]?best:hitBS[i];
        }
        fprintf(output,"Case #%d: %f\n",t+1,best);
        /*for(int i=0;i<rsize;i++){
            fprintf(output,"%f ",r[i]);
        }
        fprintf(output,"\n");*/
        delete[] hitBS;
        delete[] p;
    }
    fclose(input);
    fclose(output);
    return 0;
}

