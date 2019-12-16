#include "cstdio"
#include "algorithm"
using namespace std;
typedef long long i64;
//////////////////////////////////////////////////////////////////////
static const int MAX_N = 41;
static i64 choose[MAX_N][MAX_N];  // <-- initialize with -1
i64 Choose(int n, int k) {
  // How many sets of k objects can be taken from a group of n different
  // objects, where order doesn't matter and objects aren't replaced.
  if (n < 0 || k < 0 || k > n) return 0;
  if (k == 0 || k == n) return 1;
  if (choose[n][k] != -1) return choose[n][k];
  return choose[n][k] = Choose(n - 1, k) + Choose(n - 1, k - 1);
}
//////////////////////////////////////////////////////////////////////
double memo[MAX_N];
bool mark[MAX_N];

double f(int m, int n, int c) {
  if (c == m) return 0.0;
  double& expected = memo[c];
  if (mark[c]) return expected;
  expected = 0.0; mark[c] = true; double tp = 0.0;
  for (int i = max(n - c, 1); i <= min(n, m - c); ++i) {
    const double p = (Choose(c, n - i) * (1.0 * Choose(m - c, i))) / Choose(m, n);
    if (p > 0.0) expected += p * f(m, n, c + i), tp += p;
  }
  return (expected += 1) /= tp;
}
int main() {
  memset(choose, -1, sizeof(choose));
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    int m, n; scanf("%d %d", &m, &n);
    memset(mark, false, sizeof(mark));
    printf("Case #%d: %.10lf\n", Ti, f(m, n, 0));
  }
  return 0;
}
