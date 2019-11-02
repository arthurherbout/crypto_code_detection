#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;
#define p(x) cout<<#x<<":"<<x<<"\n"
#define lim 101

int cs,c,n,i,j,k,sz,res,x;
int X[lim],Y[lim],T[lim],C[lim];
bool B[lim],M[lim][lim];

void dfs(int i)
{
  int j;
  
  B[i]=1;
  for(j=0;j<n;j++)
    if(!B[j] && M[i][j])
	  dfs(j);
  T[--sz]=i;
}
int main()
{
  int x1,y1,x2,y2;
  scanf("%d",&cs);
  for(c=1;c<=cs;c++)
  {
    scanf("%d",&n);
	for(i=0;i<n;i++)
	  scanf("%d%d",&X[i],&Y[i]);
	memset(M,0,sizeof M);
	for(i=0;i<n;i++)
	  for(j=0;j<n;j++)
	  {
	    x1=X[i];
	    y1=Y[i];
	    x2=X[j];
	    y2=Y[j];
		if(x1<x2 && y2>=y1-1 && y2<=y1+1)
		{
		  for(k=0;k<n;k++)
		    if(k!=j && k!=i && Y[k]==y2 && X[k]>x1 & X[k]<x2)
			  break;
	      if(k==n)
		    M[i][j]=1;
		}
	  }
	memset(B,0,sizeof B);
	for(i=0,sz=n;i<n;i++)
	  if(!B[i])
	    dfs(i);
	for(i=res=0;i<n;i++)
	{
	  memset(B,0,sizeof B);
	  x=T[i];
	  for(j=0;j<n;j++)
	    if(M[j][x])
		  B[C[j]]=1;
	  if(res==0)
	  {
	    C[x]=0;
	    res++;
	  }
	  else
	  {
	    for(j=0;j<res;j++)
		  if(!B[j])
		    break;
	    if(j==res)
		{
		  C[x]=res;
		  res++;
		}
		else
		  C[x]=j;
	  }
	}
	printf("Case #%d: %d\n",c,res);
  }
  return 0;
}
