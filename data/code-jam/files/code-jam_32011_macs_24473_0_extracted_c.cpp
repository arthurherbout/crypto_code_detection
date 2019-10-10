#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;
#define p(x) cout<<#x<<":"<<x<<"\n"
#define lim 6
#define in(x,y) x>=0 && x<y

int cs,c,i,j,k,p,n,m,s,mx,x,y;
int M[lim][lim],A[lim][lim];
int dx[]={-1,-1,-1,0,0,1,1,1,0},dy[]={-1,0,1,-1,1,-1,0,1,0};

int main()
{
  scanf("%d",&cs);
  for(c=0;c<cs;c++)
  {
    scanf("%d %d",&n,&m);
	for(i=0;i<n;i++)
	  for(j=0;j<m;j++)
	    scanf("%d",&M[i][j]);
	mx=0;
	for(i=0;i<(1<<(n*m));i++)
	{
	  for(j=0;j<n;j++)
	    for(k=0;k<m;k++)
		  if((i & (1<<(m*j+k)))==0)
		    A[j][k]=0;
		  else
		    A[j][k]=1;
	  for(j=0;j<n;j++)
      {	  
	    for(k=0;k<m;k++)
		{
		  s=0;
		  for(p=0;p<9;p++)
		  {
		    x=j+dx[p];
		    y=k+dy[p];
		    if(in(x,n) && in(y,m) && A[x][y]==1)
			  s++;
		  }
		  if(s!=M[j][k])
		    break;
		}
		if(k<m)
		  break;
	  }
	  if(j==n)
	  {
	    s=0;
		for(j=0;j<m;j++)
		  if((i & (1<<(n/2*m+j)))!=0)
		    s++;
	    mx=max(mx,s);
	  }
	}
	printf("Case #%d: %d\n",c+1,mx);
  }
  return 0;
}
