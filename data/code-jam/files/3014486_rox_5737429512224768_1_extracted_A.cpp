#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>
#include <queue>

using namespace std;

int T;
int n,x,s[11111];

int main(void)
{
  scanf("%d",&T);
  for(int ttt = 1; ttt <= T; ttt++ ) {
    printf("Case #%d: ",ttt);
    scanf("%d%d",&n,&x);
    for( int i = 0; i < n; i++ ) {
      scanf("%d",s+i);
    }
    sort(s,s+n);
    int res = n;
    for( int i = n-1; i >= 0; i-- ) {
      for( int j = i-1; j >= 0; j-- ) {
        if( s[i]+s[j] <= x ) {
          s[j] = 1<<21;
          --res;
          break;
        }
      }
    }
    printf("%d\n",res);
  }
  return 0;
}
