#include "assert.h"
#include "ctype.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include "algorithm"
#include "numeric"
#include "functional"
#include "bitset"
#include "vector"
#include "list"
#include "set"
#include "map"
#include "queue"
#include "stack"
#include "string"
#include "sstream"
#include "iostream"
using namespace std;

#ifndef ONLINE_JUDGE
#pragma warning(disable:4786)  // long identifiers
#pragma warning(disable:4996)  // deprecations
#endif

typedef long long i64;

//////////////////////////////////////////////////////////////////////////////////////////

const int MOD = 10007;
int memo[100][100];
bool grid[100][100];

int f(int i, int j, int W, int H) {
  if (i == W && j == H) return 1;
  if (i > W || j > H) return 0;
  if (!grid[i][j]) return 0;
  int& count = memo[i][j];
  if (count == -1)
    count = (f(i + 2, j + 1, W, H) + f(i + 1, j + 2, W, H)) % MOD;
  return count;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("data.in", "r", stdin);
  freopen("data.out", "w", stdout);
#endif
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    int H, W, R; scanf("%d %d %d", &H, &W, &R);
    memset(grid, true, sizeof(grid));
    for (int i = 0; i < R; ++i) {
      int r, c; scanf("%d %d", &r, &c);
      grid[r - 1][c - 1] = false;
    }
    memset(memo, -1, sizeof(memo));
    const int res = f(0, 0, H - 1, W - 1);
    printf("Case #%d: %d\n", Ti, res);
  }
  return 0;
}
