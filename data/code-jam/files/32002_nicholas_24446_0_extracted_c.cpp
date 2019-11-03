
#include<stdio.h>
#include<memory.h>



int N, M, Res;
char G[12][12];
int Gp[12];int F[12][1 << 10];int num[1 << 10];
bool A(int a, int b)
{
    int i;
    for(i = 0; i < M; ++i)
        if( (a & (1 << i))  &&
            (i && (b & (1 << (i - 1))) 
            || (i < M - 1 && (b & (1 << (i + 1))))) )
            return 0;
    return 1;
}
bool B(int a, int b)
{
     int i;
     for(i = 0; i < M; ++i)
         if((a &( 1<< i)) &&(b & (1 << i))) return 0;
     return 1;
}
void DP()
{
    memset(F, 0, sizeof(F));
    F[0][0] = 0;
     int i, j, k;
     for(k = 1; k <= N; ++k)
     for(i = 0; i < (1 << M); ++i)
         for(j = 0; j < (1 << M); ++j)
         {
             if(A(i, i) && B(Gp[k - 1], i) && A(i, j) && B(Gp[k], j) && A(j, j))
             {
                 if(F[k - 1][i] != -1 && F[k - 1][i] + num[j] > F[k][j])
                     F[k][j] =  F[k - 1][i] + num[j];
             }
             if(F[k][j] > Res) Res = F[k][j];
         }
         
}

void Calc(int i)
{
     int r = 0;
     int k = i;
     while(i)
     {
         if(i & 1)
             ++r;
         i >>= 1;
     }
     num[k] = r;
}
int main()
{
    int T, i, j;
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    for(i = 1; i < 1024; ++i)
        Calc(i);
    scanf("%d", &T);
   for (int ctr = 1; ctr <= T; ctr++)
    {
        scanf("%d%d", &N, &M);
        for(i = 1; i <= N; ++i)
        {
            Gp[i] = 0;
            scanf("%s", G[i] + 1);
            for(j = 1; j <= M; ++j)
            {
                
                Gp[i] |= (G[i][j] == 'x') << (j - 1);
            }
           // printf("%d %d\n", i, Gp[i]); 
        }
        Res = 0;
        DP();
        printf("Case #%d: %d\n", ctr, Res);
    }
}
