#include<stdio.h>
#include<memory.h>
#include<iostream>
#include<algorithm>
using namespace std;
int n, m, X, Y, Z;
long long B[128];
long long A[500010];
long long tA[500010];
long long C[500010];
long long F[500010];
int Lowbit(int t) 
{ 
      return t & ( t ^ ( t - 1 ) ); 
}
long long Sum(int end) 
{ 
      long long sum = 0; 
      while(end > 0) 
      { 
          sum = (sum + C[end]) % 1000000007; 
          end -= Lowbit(end); 
      } 
      return sum; 
}
void Insert(int pos , long long num) 
{ 
      while(pos <= n) 
      { 
            C[pos] = (C[pos] + num) % 1000000007; 
            pos += Lowbit(pos); 
      } 
}

int main()
{
    int t, i, j, res, ctr = 0, low, high, mid;
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
        memcpy(tA, A, sizeof(A));
        sort(tA, tA + n);
        memset(F, 0, sizeof(F));
        memset(C, 0, sizeof(C));
        res = 0;
        for(i = 0; i < n; ++i)
        {
             low = 0, high = n - 1;
             while(high > low)
             {
                 mid = (high + low) / 2;
                 if(A[i] <= tA[mid]) high = mid;
                 else low = mid + 1;
             }
             F[i] = (1 + Sum(low)) % 1000000007;
             Insert(low + 1, F[i]);
             res = (res + F[i]) % 1000000007;
        }
        printf("Case #%d: %d\n", ++ctr, res);
    }
}

