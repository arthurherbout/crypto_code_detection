#define _CRT_SECURE_NO_DEPRECATE
#include <cstdio>
#include <algorithm>
#include <string>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <set>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

#define PROBLEM "d"

const int MAX = 1100;
const int MOD = 30031;

int n, k, p, spos;

int dp[MAX][MAX];

int main(){
  freopen(PROBLEM".in", "r", stdin);
  freopen(PROBLEM".out", "w", stdout);
  int tn, tst;
  scanf("%d", &tn);
  for (tst=1; tst<=tn; tst++){
    printf("Case #%d: ", tst);
    scanf("%d%d%d", &n, &k, &p);
    spos = ((1<<k)-1)<<(p-k);
    memset(dp, 0, sizeof(dp));
    dp[0][spos] = 1;
    int i, j, h;
    for (i=0; i<n-k; i++){
      for (j=0; j<(1<<p); j++){
        for (h=0; h<p; h++){
          if ((j & (1<<h)) == 0) continue;
          if ((h > 0) && ((j&1) > 0)) continue;
          int nj = j ^ (1<<h);
          nj >>= 1;
          nj ^= (1<<(p-1));
          dp[i+1][nj] += dp[i][j];
          dp[i+1][nj] %= MOD;
        }
      }
    }
    printf("%d\n", dp[n-k][spos]);
  }
  return 0;  
}