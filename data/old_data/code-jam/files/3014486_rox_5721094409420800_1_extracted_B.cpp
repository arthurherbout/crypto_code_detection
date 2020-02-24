#include <cstdio>
#include <map>
#include <algorithm>
#include <functional>
#include <cstring>

using namespace std;

int T,n,a[1111];
pair<int,int> b_[1111];
int b[1111],id[1111];
int m[1111];
int re[1111];
int dp[1111];

int solve(int p,int l,int r)
{
  int res = 1<<21;
  if( p == n ) return 0;
  if( dp[p] >= 0 ) return dp[p];
  for( int i = 0; i <= m[p]; i++ ) {
    int c = 0;
    //printf("%d %d\n",p,id[p]);
    for( int j = 0; j < i; j++ ) {
      for( int k = 0; k < id[p+j]; k++ ) {
        if( a[k] > b[p] ) ++c;
      }
    }
    for( int j = i; j < m[p]; j++ ) {
      for( int k = id[p+j]+1; k < n; k++ ) {
        if( a[k] > b[p] ) ++c;
      }
    }
    //printf("%d %d %d\n",p,i,c);
    res = min(res,solve(p+m[p],l+i,r-m[p])+c);
    /*
    for( int j = 0; j < i; j++ ) {
      re[l+j] = b[p];
    }
    for( int j = i; j <= m[p]; j++ ) {
      re[r-j] = b[p];
    }
    //*/
  }
  return dp[p] = res;
}

int main(void)
{
  scanf("%d",&T);
  for( int T_ = 1; T_<=T; T_++ ) {
    printf("Case #%d: ",T_);
    memset(dp,-1,sizeof(dp));
    scanf("%d",&n);
    for( int i = 0; i < n; i++ ) {
      scanf("%d",a+i);
      b_[i]=make_pair(a[i],i);
    }
    sort(b_,b_+n);
    for( int i = 0; i < n; i++ ) {
      b[i] = b_[i].first;
      id[i] = b_[i].second;
    }
    for( int i = 0; i < n; i++ ) m[i] = 0;
    m[0] += 1;
    for( int i = 1; i < n; i++ ) {
      m[i] = 1;
      if( b[i] == b[i-1] ) m[i] += m[i-1];
      else {
        for( int j = i-1; j >= 0 && m[j+1]-1 == m[j]; j-- ) {
          m[j] = m[j+1];
        }
      }
    }
    //for( int i = 0; i < n; i++ ) printf("%d ",m[i]);puts("");
    printf("%d\n",solve(0,0,n-1));
  }
  return 0;
}
