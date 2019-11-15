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

int GetMinCost(int n, const char* str, int len) {
  vector<int> p(n);
  for (int i = 0; i < n; ++i)
    p[i] = i;
  int minCost = INT_MAX;
  do {
    int cost = 1;
    char last = str[p[0]];
    for (int i = 1; i < len; ++i) {
      const char c = str[(i / n) * n + p[i % n]];
      if (c != last) ++cost;
      last = c;
    }
    minCost = min(minCost, cost);
  } while (next_permutation(p.begin(), p.end()));
  return minCost;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("data.in", "r", stdin);
  freopen("data.out", "w", stdout);
#endif
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    int n; scanf("%d", &n);
    static char str[60000]; scanf("%s", str);
    const int len = (int)strlen(str);
    const int res = GetMinCost(n, str, len);
    printf("Case #%d: %d\n", Ti, res);
  }
  return 0;
}
