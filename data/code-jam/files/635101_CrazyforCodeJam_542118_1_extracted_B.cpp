#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAXN 55

int X[MAXN],V[MAXN],f[MAXN],b[MAXN];

int main()
{
    freopen("B-large.in","r",stdin);
    freopen("B-large.out","w",stdout);
    int C,
    N,K,B,T;
    scanf("%d",&C);
    for (int i=0;i<C;i++)
    {
    scanf("%d%d%d%d",&N,&K,&B,&T);
    for (int j=0;j<N;j++)
    scanf("%d",X+j);
    for (int j=0;j<N;j++)
    scanf("%d",V+j);
    
    int flag = 0,sum=0,cnt;
    memset(f,0,sizeof(f));    
    memset(b,0,sizeof(b));
    for (int j=0;j<N;j++)
    {
        if (X[j]+T*V[j]>=B)
        {f[j]=1;sum++;}
    }
    if (K==0)
    {printf("Case #%d: 0\n",i+1);continue;}
    if (sum<K)
    printf("Case #%d: IMPOSSIBLE\n",i+1);
    else
    {
        cnt = 0;
        for (int i=N-1;i>=0;i--)
        {
            if (f[i]==1)
            cnt++;
            else
            b[cnt]++;
        }
        int res=0;
        for (int i=K;i>=1;i--)
        {
            res += i*b[K-i];
        }
        printf("Case #%d: %d\n",i+1,res);
    }
    }
return 0;
}
