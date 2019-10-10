#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;
#define p(x) cout<<#x<<":"<<x<<"\n"
#define lim 10010
#define lim2 10010

int cs,c,i,j,k,s,mx,n,x,y,z;
int A[lim],B[lim],C[lim],X2[lim2],Y2[lim2],Z2[lim2];
int X[lim2][lim2],Y[lim2][lim2],Z[lim2][lim2];

int main()
{
  scanf("%d",&cs);
  for(c=0;c<cs;c++)
  {
    scanf("%d",&n);
	memset(X,0,sizeof X);
	memset(Y,0,sizeof Y);
	memset(Z,0,sizeof Z);
	memset(X2,0,sizeof X2);
	memset(Y2,0,sizeof Y2);
	memset(Z2,0,sizeof Z2);
	for(i=0;i<n;i++)
	{
	  scanf("%d %d %d",&A[i],&B[i],&C[i]);
	  X[A[i]][B[i]]++;
	  Y[A[i]][C[i]]++;
	  Z[B[i]][C[i]]++;
	  X2[A[i]]++;
	  Y2[B[i]]++;
	  Z2[C[i]]++;
	}
	for(i=10000;i>=0;i--)
	{
	  for(j=10000;j>=0;j--)
	  {
	    X[i][j]+=X[i][j+1]+X[i+1][j]-X[i+1][j+1];
	    Y[i][j]+=Y[i][j+1]+Y[i+1][j]-Y[i+1][j+1];
	    Z[i][j]+=Z[i][j+1]+Z[i+1][j]-Z[i+1][j+1];
	  }
	  X2[i]+=X2[i+1];
	  Y2[i]+=Y2[i+1];
	  Z2[i]+=Z2[i+1];
	}
	mx=0;
	for(i=0;i<n;i++)
	  for(j=0;j<n;j++)
	  {
	    x=A[i];
	    y=B[j];
		z=10000-x-y;
		if(z>=0)
		{
		  s=n;
		  s-=X2[x+1];
		  s-=Y2[y+1];
		  s-=Z2[z+1];
		  s+=X[x+1][y+1];
		  s+=Y[x+1][z+1];
		  s+=Z[y+1][z+1];
		  mx=max(mx,s);
		}
	  }
	printf("Case #%d: %d\n",c+1,mx);
  }
  return 0;
}
