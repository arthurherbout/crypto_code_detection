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

const int di[] = {-1, -1, -1, 0, 0, +1, +1, +1};
const int dj[] = {-1, 0, +1, -1, +1, -1, 0, +1};
char grid[20][20];
int m, n;
bool memo[4][4][1 << 16];
bool mark[4][4][1 << 16];

int GetIndex(int i, int j) {return i * n + j;}

pair<int, int> FindKing() {
  for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j)
    if (grid[i][j] == 'K') return pair<int, int>(i, j);
  fprintf(stderr, "ERROR: King not found!\n");
  return make_pair(-1, -1);
}
int GetMask() {
  int mask = 0;
  for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j)
    if (grid[i][j] == '.') mask |= 1 << GetIndex(i, j);
  return mask;
}

bool f(int i, int j, int mask) {
  bool& win = memo[i][j][mask];
  if (mark[i][j][mask]) return win;
  win = false; mark[i][j][mask] = true;
  for (int k = 0; k < 8 && !win; ++k) {
    const int ni = i +di[k], nj = j + dj[k];
    if (ni >= 0 && ni < m && nj >= 0 && nj < n)
      if (mask & (1 << GetIndex(ni, nj)))
        win = !f(ni, nj, mask & ~(1 << GetIndex(ni, nj)));
  }
  return win;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("data.in", "r", stdin);
  freopen("data.out", "w", stdout);
#endif
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    scanf("%d %d", &m, &n);
    for (int i = 0; i < m; ++i)
      scanf("%s", grid[i]);
    const pair<int, int> start = FindKing();
    const int mask = GetMask();
    memset(mark, false, sizeof(mark));
    const bool aWin = f(start.first, start.second, mask);
    printf("Case #%d: %s\n", Ti, aWin ? "A" : "B");
  }
  return 0;
}
