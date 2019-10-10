#include "cmath"
#include "cstdio"
#include "algorithm"
#include "map"
#include "numeric"
#include "queue"
#include "set"
#include "sstream"
#include "string"
#include "utility"
#include "vector"
using namespace std;
typedef long long i64;

const int MOD = 10009;
int fact[11];
int dict[100][26];
int n;

template <class T>
T BigPowerMod(T b, T p, T m) {  // (b^p) (mod c)
  T x = 1, y = b % m;
  for (; p > 0; p /= 2) {
    if (p % 2 == 1)
      x = (x * y) % m;
    y = (y * y) % m;
  }
  return x % m;
}
int BitCount(int x) {
  int cnt = 0;
  for (; x > 0; x &= x - 1, ++cnt);
  return cnt;
}
vector<vector<pair<int, int> > > ParsePoly(string s) {
  for (int i = 0; i < (int)s.size(); ++i)
    if (s[i] == '+') s[i] = ' ';
  vector<vector<pair<int, int> > > poly;
  istringstream iss(s);
  for (string x; iss >> x;) {
    vector<pair<int, int> > term;
    char last = x[0]; int cnt = 1;
    for (int i = 1; i < (int)x.size(); ++i)
      if (x[i] == last) ++cnt;
      else {
        term.push_back(pair<int, int>(last - 'a', cnt));
        last = x[i], cnt = 1;
      }
    term.push_back(pair<int, int>(last - 'a', cnt));
    poly.push_back(term);
  }
  return poly;
}
int Eval(const vector<vector<pair<int, int> > >& poly, const vector<int>& v) {
  int sum = 0;
  for (int i = 0; i < (int)poly.size(); ++i) {
    int term = 1;
    for (int j = 0; j < (int)poly[i].size(); ++j)
      (term *= BigPowerMod(v[poly[i][j].first], poly[i][j].second, MOD)) %= MOD;
    (sum += term) %= MOD;
  }
  return sum;
}
int EvalCounts(const vector<vector<pair<int, int> > >& poly, const vector<int>& counts) {
  vector<int> v(26, 0);
  for (int i = 0; i < n; ++i) if (counts[i] > 0)
    for (int j = 0; j < 26; ++j) v[j] += counts[i] * dict[i][j];
  int res = fact[accumulate(counts.begin(), counts.end(), 0)];
  for (int i = 0; i < (int)counts.size(); ++i)
    res /= fact[counts[i]];
  return (Eval(poly, v) * res) % MOD;
}
int EvalAll(const vector<vector<pair<int, int> > >& poly, vector<int>& counts, int last, int left) {
  if (left == 0) return EvalCounts(poly, counts);
  if (last >= n) return 0;
  int sum = EvalAll(poly, counts, last + 1, left);
  ++counts[last];
  (sum += EvalAll(poly, counts, last, left - 1)) %= MOD;
  --counts[last];
  return sum;
}
int main() {
  fact[0] = 1;
  for (int i = 1; i <= 10; ++i)
    fact[i] = fact[i - 1] * i;
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    fprintf(stderr, "Ti=%d\n", Ti);
    char p[100000]; scanf("%s", p);
    vector<vector<pair<int, int> > > poly = ParsePoly(p);
    int m; scanf("%d %d", &m, &n);
    for (int i = 0; i < n; ++i) {
      char str[100000]; scanf("%s", str);
      const int len = (int)strlen(str);
      memset(dict[i], 0, sizeof(dict[0]));
      for (int j = 0; j < len; ++j)
        ++dict[i][str[j] - 'a'];
    }
    vector<int> sums(m, 0), counts(n, 0);
    for (int i = 0; i < m; ++i)
      sums[i] = EvalAll(poly, counts, 0, i + 1);
    printf("Case #%d:", Ti);
    for (int i = 0; i < (int)sums.size(); ++i)
      printf(" %d", sums[i]);
    printf("\n");
  }
  return 0;
}
