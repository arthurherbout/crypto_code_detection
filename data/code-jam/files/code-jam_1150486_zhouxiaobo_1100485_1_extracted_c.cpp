#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

int m,prime[1000005];
bool isprime[1000005];

int Prime(int n)
{
  int i,j,t,m=0;
  isprime[1]=false;
  for(i=2;i<=n;i++) isprime[i]=true;
  for(i=2;i<=n;i++)
  {
    if (isprime[i]) prime[++m]=i;
    t=n/i;
    for(j=1;prime[j]<=t;j++)
    {
      isprime[i*prime[j]]=false;
      if (i%prime[j]==0) break;
    }
  }
  return m;
}

long long solve(long long n)
{
    if (n==1) return 0;
    long long res=1,t;
    for(int i=1;i<=m;++i)
    {
        t=prime[i];
        while(t<=n/prime[i])
        {
            ++res;
            t*=prime[i];
        }
    }
    return res;
}

int main(void)
{
    freopen("C-large.in","r",stdin);
    freopen("C-large.out","w",stdout);
    int T,u;
    long long n;
    m=Prime(1000000);
    scanf("%d",&T);
    for(u=1;u<=T;++u)
    {
        scanf("%I64d",&n);
        printf("Case #%d: %I64d\n",u,solve(n));
    }
    return 0;
}
