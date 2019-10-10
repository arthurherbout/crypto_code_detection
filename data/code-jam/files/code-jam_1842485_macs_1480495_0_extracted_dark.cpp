#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <set>
#include <queue>
using namespace std;
#define p(x) cout<<#x<<":"<<x<<"\n"
#define ull unsigned long long
#define lim 11

int c,cs,n,m,i,j,k,p,x,y,j2,k2,s;
ull mask,mask2;
int dx[]={0,1,0};
int dy[]={1,0,-1};
queue <ull> Q;
set <ull> S;
char A[lim][lim];

ull f(int x,int y)
{
  return 1ull<<((m-2)*(x-1)+y-1);
}
void dfs(int i,int j)
{
  int x,y,k;

  mask|=f(i,j);
  s++;
  for(k=0;k<3;k++)
  {
    x=i-dx[k];
    y=j-dy[k];
    if(A[x][y]!='#' && !(mask&f(x,y)))
      dfs(x,y);
  }
}
void push(ull x)
{
  if(S.find(x)==S.end())
  {
    S.insert(x);
    Q.push(x);
  }
}
int main()
{
  scanf("%d",&cs);
  for(c=1;c<=cs;c++)
  {
    scanf("%d%d",&n,&m);
    for(i=0;i<n;i++)
      scanf("%s",A[i]);
    printf("Case #%d:\n",c);
    for(i='0';i<='9';i++)
    {
      for(j=0;j<n;j++)
      {
        for(k=0;k<m && A[j][k]!=i;k++);
        if(k<m)
          break;
      }
      if(j==n)
        break;
      j2=j;
      k2=k;
      mask=0;
      s=0;
      dfs(j,k);
      for(j=1,mask2=0;j+1<n;j++)
      {
        for(k=1;k+1<m && !(mask&f(j,k));k++);
        if(k+1<m)
          mask2|=f(j,k);
      }
      S.clear();
      push(mask2);
      while(!Q.empty())
      {
        mask=Q.front();
        Q.pop();
        for(p=0;p<3;p++)
        {
          for(j=1,mask2=0;j+1<n;j++)
          {
            for(k=1;k+1<m;k++)
              if(mask&f(j,k))
              {
                x=j+dx[p];
                y=k+dy[p];
                if(A[x][y]!='#')
                {
                  if(x>j2)
                    break;
                  mask2|=f(x,y);
                }
                else
                  mask2|=f(j,k);
              }
            if(k+1<m)
              break;
          }
          if(j+1==n)
            push(mask2);
        }
      }
      printf("%c: %d %s\n",i,s,S.find(f(j2,k2))==S.end()?"Unlucky":"Lucky");
    }
  }
  return 0;
}
