#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;
#define p(x) cout<<#x<<":"<<x<<"\n"
#define lim 16

int cs,c,n,m,i;
char A[lim][lim];
int M[lim*lim][(1<<15)+1];

int solve(int i,int mask)
{
  int &res=M[i][mask],x,y,z,z2,f=-1;
   
  if(res==-1)
  {
    x=i/m;
    y=i%m;
    if(i==n*m)
	  res=__builtin_popcount(mask);
	else
	  if(A[x][y]=='.')
	  {
	    z=0;
	    if(y!=0 && (mask & (1<<0))!=0)
		  z++;
		if((mask & (1<<(m-1)))!=0)
		  z++;
/*		p(i+1);
		p((2*mask)%(1<<m));*/
	    res=solve(i+1,(2*mask)%(1<<m))+z;
	  }
	  else if(A[x][y]=='#')
	  {
	    z=0;
	    if(y!=0 && (mask & (1<<0))==0)
		  z++;
		if((mask & (1<<(m-1)))==0)
		  z++;
		if(y==m-1 || y==0)
		  z++;
	    res=solve(i+1,(2*mask+1)%(1<<m))+z;
      }
	  else
	  {
	    z=0;
	    if(y!=0 && (mask & (1<<0))!=0)
		  z++;
		if((mask & (1<<(m-1)))!=0)
		  z++;
		z2=0;
	    if(y!=0 && (mask & (1<<0))==0)
		  z2++;
		if((mask & (1<<(m-1)))==0)
		  z2++;
		if(y==m-1 || y==0)
		  z2++;
		res=max(solve(i+1,(2*mask)%(1<<m))+z,solve(i+1,(2*mask+1)%(1<<m))+z2);
      }
//	printf("%d %d %d %d %d %d\n",i,mask,res,x,y,z);
  }
  return res;
}
int main()
{
  scanf("%d",&cs);
  for(c=0;c<cs;c++)
  {
    scanf("%d %d",&n,&m);
	for(i=0;i<n;i++)
	  scanf("%s",A[i]);
    memset(M,-1,sizeof M);
	printf("Case #%d: %d\n",c+1,solve(0,0));
  }
  return 0;
}
