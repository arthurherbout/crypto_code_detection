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
    int n; scanf("%d", &n);
    vector<i64> v1(n), v2(n);
    for (int i = 0; i < n; ++i)
      scanf("%lld", &v1[i]);
    for (int i = 0; i < n; ++i)
      scanf("%lld", &v2[i]);
    sort(v1.begin(), v1.end(), less<i64>());
    sort(v2.begin(), v2.end(), greater<i64>());
    i64 dot = 0;
    for (int i = 0; i < n; ++i)
      dot += v1[i] * v2[i];
    printf("Case #%d: %lld\n", Ti, dot);
  }
  return 0;
}
