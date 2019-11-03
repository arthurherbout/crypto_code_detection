#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;
#define p(x) cout<<#x<<":"<<x<<"\n"
#define in(x,y) x>=0 && x<y
#define lim 3
#define lim2 101

int cs,c,n,m,s,x,y;
int X[lim],Y[lim];
bool B[lim2][lim2];

int dfs(int x,int y)
{
  int x2,y2;

  B[x][y]=1;
  s++;
  x2=x+X[0];
  y2=y+Y[0];
  if(in(x2,n) && in(y2,m) && !B[x2][y2])
    dfs(x2,y2);
  x2=x+X[1];
  y2=y+Y[1];
  if(in(x2,n) && in(y2,m) && !B[x2][y2])
    dfs(x2,y2);
  return 0;
}
int main()
{
  scanf("%d",&cs);
  for(c=0;c<cs;c++)
  {
    scanf("%d %d",&n,&m);
	scanf("%d %d",&X[0],&Y[0]);
	scanf("%d %d",&X[1],&Y[1]);
	scanf("%d %d",&x,&y);
	s=0;
	memset(B,0,sizeof B);
	dfs(x,y);
	printf("Case #%d: %d\n",c+1,s);
  }
  return 0;
}
