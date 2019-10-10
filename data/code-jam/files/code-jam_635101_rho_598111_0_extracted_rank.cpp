#include<stdio.h>
#define MOD 100003

int main(void)
{
    FILE* fin = fopen("rank.in", "r");
    FILE* fout = fopen("rank.out", "w");  
    long vals[501][501];
    long c[501][501];
    for(int i=0; i<=500; i++)
    {
        c[i][0]=1;
        for(int j=0; j<i; j++)
        {
            c[i][j+1]=(c[i][j]*(i-j)/(j+1))%MOD;
        }
    }
    printf("FINISH1\n");
    for(int i=2; i<=500; i++)
    {
        vals[i][1]=1;    
        for(int j=2; j<i; j++)
        {
            long sum=0;
            for(int k=j-1; k>=2*j-i && k>=1; k--)
            {
                sum=(sum+vals[j][k]*c[i-j-1][j-k-1])%MOD;
            }
            vals[i][j]=sum;
            //printf("%d %d = %ld\n", i, j, sum);
        }
    }
    printf("FINISH\n");
    long n;
    fscanf(fin, "%ld", &n);
    for(long i=1; i<=n; i++)
    {
        long a;
        fscanf(fin, "%ld", &a);
        long r=0;
        for(long j=1; j<a; j++)
            r+=vals[a][j];
        fprintf(fout, "Case #%d: %ld\n",  i, r%MOD);
   }
   //getchar();
}

