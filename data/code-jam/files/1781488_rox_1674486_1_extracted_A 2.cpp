#include <cstdio>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
int n;
vector<int> g[1111];
int in[1111];
bool solve(int a)
{
  int dp[1111]={};
  queue<int>q; q.push(a);
  while(!q.empty()){
    int p=q.front(); q.pop();
    dp[p]++;
    if(dp[p]>=2)return true;
    for(int i=0;i<int(g[p].size());i++){
      q.push(g[p][i]);
    }
  }
  return false;
}
int main(void)
{
  int t; scanf("%d",&t);
  for(int T=1;T<=t;T++){
    printf("Case #%d: ",T);
    scanf("%d",&n);
    fill(in,in+n,0);
    for(int i=0;i<n;i++){
      int k;
      scanf("%d",&k);
      in[i]=k;
      for(int j=0;j<k;j++){
        int f; scanf("%d",&f); --f;
        g[f].push_back(i);
      }
    }
    for(int i=0;i<n;i++){
      if(!in[i]){
        if(solve(i)){
          puts("Yes");
          goto NEXT;
        }
      }
    }
    puts("No");
  NEXT:;
    for(int i=0;i<n;i++) g[i].clear();
  }
}
