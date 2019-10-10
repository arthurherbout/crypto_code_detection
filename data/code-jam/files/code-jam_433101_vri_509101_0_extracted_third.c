#include <stdio.h>
int main() {
    long R, k, g[1000],E,ppl;
    int N,t;
    int i,j,p,q,r,start;
    FILE *fin, *fout;
    fin=fopen("input3","r");
    fout=fopen("output3", "w");
    fscanf(fin,"%d",&t);
    for(i=1;i<=t;i++) {
        fscanf(fin, "%ld%ld%d", &R, &k, &N);
        for(j=0;j<N;j++) {
            fscanf(fin, "%ld", &g[j]);
        }
        E=0;
        start=0;
        for(p=0;p<R;p++) {
            ppl=0;
            for(q=start;;){
                
                if(g[q]+ppl<=k) {
                     ppl+=g[q];
                     q++;
                     if(q==N) {
                         q=0;
                     }
                     if(q==start) {

                         break;
                     }
      //               start=q;
                }else {
                    start=q;
                    break;
                }
            }
            E+=ppl;
        }


        fprintf(fout, "Case #%d: %ld\n", i, E);
    }    
    fclose(fin);
    fclose(fout);
    return 0;
}
