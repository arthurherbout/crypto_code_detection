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

double prob[6][4];
int n;

void GetAllChances(int q, double p, vector<double>& v) {
  if (q >= n) v.push_back(p);
  else for (int i = 0; i < 4; ++i)
    GetAllChances(q + 1, p * prob[q][i], v);
}

double f(int m) {
  vector<double> v; GetAllChances(0, 1.0, v);
  if (m >= (int)v.size()) return 1.0;
  sort(v.begin(), v.end(), greater<double>());
  return accumulate(v.begin(), v.begin() + m, 0.0);
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("data.in", "r", stdin);
  freopen("data.out", "w", stdout);
#endif
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    int m; scanf("%d %d", &m, &n);
    for (int i = 0; i < n; ++i) for (int j = 0; j < 4; ++j)
      scanf("%lf", &prob[i][j]);
    const double res = f(m);
    printf("Case #%d: %.10lf\n", Ti, res);
  }
  return 0;
}
