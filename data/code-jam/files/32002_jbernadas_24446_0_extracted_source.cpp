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

int memo[10][1 << 10];
int grid[10];
int M, N;

int bitcount(int mask) {
  int cnt = 0;
  for (int i = 0; i < N; ++i)
    if (mask & (1 << i)) ++cnt;
  return cnt;
}

int GetMenaced(int mask) {
  int menaced = 0;
  for (int i = 0; i < N; ++i) if (mask & (1 << i)) {
    if (i - 1 >= 0) menaced |= 1 << (i - 1);
    if (i + 1 < N) menaced |= 1 << (i + 1);
  }
  return menaced;
}

int f(int row, int mask) {
  if (row >= M) return 0;
  int& best = memo[row][mask];
  if (best != -1) return best;
  best = 0;
  const int impossible = grid[row] | mask;
  for (int put = 0; put < (1 << N); ++put) if (!(put & impossible)) {
    bool ok = true;
    for (int j = 0; j < N - 1 && ok; ++j)
      if ((put & (1 << j)) && put & (1 << (j + 1))) ok = false;
    if (!ok) continue;
    const int menaced = GetMenaced(put);
    best = max(best, f(row + 1, menaced) + bitcount(put));
  }
  return best;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("data.in", "r", stdin);
  freopen("data.out", "w", stdout);
#endif
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    scanf("%d %d", &M, &N);
    for (int i = 0; i < M; ++i) {
      static char str[20]; scanf("%s", &str);
      grid[i] = 0;
      for (int j = 0; j < N; ++j)
        if (str[j] == 'x') grid[i] |= 1 << j;
    }
    memset(memo, -1, sizeof(memo));
    const int res = f(0, 0);
    printf("Case #%d: %d\n", Ti, res);
  }
  return 0;
}
