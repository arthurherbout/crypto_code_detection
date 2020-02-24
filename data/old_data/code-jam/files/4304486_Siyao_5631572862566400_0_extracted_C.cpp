#include <iostream>
#include <algorithm>
#include <string.h>
using namespace std;

int N;
int F[1100];
bool tag[1100];
int be;
int ans=0;
int  tmp[1100];
void dfs(int step, int num, int pre)
{
  tmp[step]=num;
  if(step>N) return;
   if(num==be)
   if(step>ans) 
    {
      ans=step;
      // for(int i=1;i<=step;++i) printf("%d->",tmp[i]);
      //   printf("\n");
    }
  if(tag[num]==1) return;
  tag[num]=1;
  if(F[num]!=pre) dfs(step+1, F[num],num);
  else
  {
   
    for(int i=1;i<=N;++i)
    {
      if(tag[i]==0||i==be)
      {
       	dfs(step+1,i,num);
      }
    }
  }
  tag[num]=0;
}

int main()
{
  int  T;
  scanf("%d",&T);
  for(int i=1;i<=T;++i)
  {
    printf("Case #%d: ", i);
    // input
    scanf("%d",&N);
    ans=0;
   
    for(int j=1;j<=N;++j) scanf("%d",&F[j]);
    for(be=1;be<=N;++be)
      {
	 for(int j=0;j<=N;++j) tag[j]=0;
	 tag[be]=1;
	 dfs(1,F[be],be);
      }
    printf("%d\n",ans);
    // deal 
  
  }
  //printf("%d\n",maxt);
  return 0;
}
