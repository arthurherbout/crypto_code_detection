#include <stdio.h>
double fact ( int x) {
int f=1, i;
for (i=1;i<=x;i++) f=f*x;
return f;
}

int main() {
    FILE * filein, *fileout;
    int t,n,i,j,s,k,min, temp=0;
    double e=0.0;
    int c[1001];int d[1001];
    filein=fopen("input", "r");
    fileout=fopen("output","w");
    fscanf(filein,"%d", &t);
    for( i=0;i<t;i++) {
        fscanf(filein, "%d",&n);
        s=0;
        e=0;
        for (j=0;j<n;j++) {
            fscanf(filein, "%d", &c[j]);
            d[j]=c[j];
        }
        for (j=0;j<n;j++) {
            min=j;
            for (k=j+1;k<n;k++) {
                if(d[k]<d[min]){
                    min=k;
                }
            }
            if(min!=j) {
                temp=d[min];
                d[min]=d[j];
                d[j]=temp;
                s++;
            }
        }
        if (1.0*fact(s)==1.0) e=1.00;
        else e=1.0*fact(s)/(1.0*fact(s)-1.0);
        fprintf(fileout, "Case #%d: %.6lf\n", i+1, e);
    } 
 
    fclose(filein);
    fclose(fileout);
    return 0;
}

