#include "cmath"
#include "cstdio"
#include "algorithm"
#include "map"
#include "set"
#include "string"
#include "vector"
using namespace std;
typedef long long i64;
const int INFINITE = 0x3f3f3f3f;

bool isLower[16][16];
int memo[17][1 << 16];
bool mark[17][1 << 16];

bool IsLower(const vector<int>& a, const vector<int>& b) {
  bool ok = true;
  for (int i = 0; i < (int)a.size() && ok; ++i)
    ok = a[i] < b[i];
  return ok;
}
int f(int n, int last, int mask) {
  if (mask == 0) return last == n ? 0 : 1;
  int& best = memo[last][mask];
  if (mark[last][mask]) return best;
  best = INFINITE, mark[last][mask] = true;
  if (last == n) {
    for (int j = 0; j < n; ++j) if (mask & (1 << j))
      best = min(best, f(n, j, mask & ~(1 << j)));
  } else {
    best = min(best, f(n, n, mask) + 1);
    for (int j = 0; j < n; ++j)
      if ((mask & (1 << j)) && isLower[last][j])
        best = min(best, f(n, j, mask & ~(1 << j)));
  }
  return best;
}
int main() {
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    int n, m; scanf("%d %d", &n, &m);
    vector<vector<int> > stocks(n, vector<int>(m));
    for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j)
      scanf("%d", &stocks[i][j]);
    sort(stocks.begin(), stocks.end());
    memset(isLower, false, sizeof(isLower));
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
      isLower[i][j] = IsLower(stocks[i], stocks[j]);
    memset(mark, false, sizeof(mark));
    printf("Case #%d: %d\n", Ti, f(n, n, (1 << n) - 1));
  }
  return 0;
}
