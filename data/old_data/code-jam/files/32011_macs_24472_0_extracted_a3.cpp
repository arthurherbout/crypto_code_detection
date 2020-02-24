#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;
#define p(x) cout<<#x<<":"<<x<<"\n"
#define lim 10001

int cs,c2,n,i,j,s,a,b,c,mx;
int A[lim],B[lim],C[lim];

int main()
{
  scanf("%d",&cs);
  for(c2=0;c2<cs;c2++)
  {
    scanf("%d",&n);
    for(i=0;i<n;i++)
	  scanf("%d %d %d",&A[i],&B[i],&C[i]);
	mx=0;
    for(i=0;i<(1<<n);i++)
	{
	  s=0;
	  a=0;
	  b=0;
	  c=0;
	  for(j=0;j<n;j++)
	    if((i & (1<<j))!=0)
		{
	      a=max(a,A[j]);
	      b=max(b,B[j]);
	      c=max(c,C[j]);
          s++;
		}
	  if(a+b+c<=10000)
	  {
	    mx=max(mx,s);
      }
	}
	printf("Case #%d: %d\n",c2+1,mx);
  }
  return 0;
}
