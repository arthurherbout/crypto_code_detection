#include<stdio.h>
#include<memory.h>
int n, m, X, Y, Z;
long long B[128];
int A[500010];
int F[500010];

int main()
{
    int t, i, j, res, ctr = 0;
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
    scanf("%d", &t);
    while(t--)
    {
        scanf("%d%d%d%d%d", &n, &m, &X, &Y, &Z);
        for(i = 0; i < m; ++i)
            scanf("%I64d", &B[i]);
        for(i = 0; i < n; ++i)
        {
              A[i] = B[i % m];
              B[i % m] = (X * B[i % m] + (long long)Y * (i + 1)) % Z;
        }
        memset(F, 0, sizeof(F));
        res = 0;
        for(i = 0; i < n; ++i)
        {
             F[i] = 1;
             for(j = 0; j < i; ++j)
                 if(A[i] > A[j])
                     F[i] = (F[i] + F[j]) % 1000000007; 
             res = (res + F[i]) % 1000000007;
        }
        printf("Case #%d: %d\n", ++ctr, res);
    }
}