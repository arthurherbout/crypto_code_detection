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
using namespace std;

#ifndef ONLINE_JUDGE
#pragma warning(disable:4786)  // long identifiers
#pragma warning(disable:4996)  // deprecations
#endif

typedef long long i64;

//////////////////////////////////////////////////////////////////////////////////////////

int main() {
#ifndef ONLINE_JUDGE
  freopen("data.in", "r", stdin);
  freopen("data.out", "w", stdout);
#endif
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    int n, m; scanf("%d %d", &n, &m);
    vector<vector<pair<int, int> > > v(m);
    for (int i = 0; i < m; ++i) {
      int cnt; scanf("%d", &cnt);
      for (int j = 0; j < cnt; ++j) {
        int milkshake, malted; scanf("%d %d", &milkshake, &malted);
        v[i].push_back(make_pair(milkshake - 1, malted));
      }
    }
    int bestCount = n + 1, config = -1;
    for (int i = 0; i < (1 << n); ++i) {
      bool ok = true;
      for (int j = 0; j < m && ok; ++j) {
        bool found = false;
        for (int k = 0; k < (int)v[j].size() && !found; ++k)
          found = (((i & (1 << v[j][k].first)) >> v[j][k].first) & 1) == v[j][k].second;
        ok = found;
      }
      if (ok) {
        int bitCount = 0;
        for (int k = i; k != 0; k &= k - 1, ++bitCount);
        if (bitCount < bestCount) bestCount = bitCount, config = i;
      }
    }
    if (config == -1) printf("Case #%d: IMPOSSIBLE\n", Ti);
    else {
      printf("Case #%d:", Ti);
      for (int i = 0; i < n; ++i)
        printf(" %d", ((config & (1 << i)) >> i) & 1);
      printf("\n");
    }
  }
  return 0;
}
