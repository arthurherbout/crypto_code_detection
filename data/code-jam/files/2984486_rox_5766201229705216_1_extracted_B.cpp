#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> g[1111];
int dp[1111][1111];

int solve(int u,int pre)
{
  if( dp[u][pre] >= 0 ) return dp[u][pre];
  int res = 1;
  for( int i = 0; i < int(g[u].size()); i++ ) {
    if( g[u][i] == pre ) continue;
    for( int j = i+1; j < int(g[u].size()); j++ ) {
      if( g[u][j] == pre ) continue;
      res = max(res,solve(g[u][i],u)+solve(g[u][j],u)+1);
    }
  }
  return dp[u][pre]=res;
}

int main(void)
{
  int T; scanf("%d",&T);
  int n;
  for( int tt = 1; tt <= T; tt++ ) {
    printf("Case #%d: ",tt);
    scanf("%d",&n);
    for( int i = 0; i < n-1; i++ ) {
      int f,t; scanf("%d%d",&f,&t);
      g[f].push_back(t);
      g[t].push_back(f);
    }
    int res = 1;
    memset(dp,-1,sizeof(dp));
    for( int i = 1; i <= n; i++ ) {
      int ret = solve(i,0);
      //printf("%d %d\n",i,ret);
      if( res < ret ) res = ret;
    }
    printf("%d\n",n-res);
    for( int i = 1; i <= n; i++ ) g[i].clear();
  }
  return 0;
}
