#include "cmath"
#include "cstdio"
#include "algorithm"
#include "map"
#include "numeric"
#include "queue"
#include "set"
#include "string"
#include "utility"
#include "vector"
using namespace std;
typedef long long i64;
const i64 MOD = 1000000007LL;
//////////////////////////////////////////////////////////////////////////////////////////
template <class T>
T Modulo(T x, T n) {
  return (n + x % n) % n;
}
template <class T>
T ExtendedGCD(T a, T b, T& x, T& y) {
  // xa + yb = GCD(a, b)
  if (b > a) return ExtendedGCD(b, a, y, x);
  if (b == 0) return x = 1, y = 0, a;
  T x1, y1, d = ExtendedGCD(b, a % b, x1, y1);
  return x = y1, y = x1 - (a / b) * y1, d;
}
template <class T>
pair<T, bool> ModularInverse(T a, T b, T n) {
  // ax = b (mod n), all solutions are of the form x + k * (n / GCD(a, n))
  a = Modulo(a, n), b = Modulo(b, n);
  T x, y, d = ExtendedGCD(a, n, x, y);
  if (b % d != 0) return make_pair(0, false);
  else return make_pair(Modulo((x * b) / d, n), true);
}
//////////////////////////////////////////////////////////////////////////////////////////
i64 fact[71] = {1}, invf[71] = {1};
i64 fmemo[19][71][71][2], gmemo[71][71][10000];
bool fmark[19][71][71][2], gmark[71][71][10000];

i64 g(int b, int n, int start, int sum) {
  if (n < 0 || sum < 0) return 0;
  if (n == 0) return sum == 0 ? 1 : 0;
  i64& count = gmemo[n][start][sum];
  if (gmark[n][start][sum]) return count;
  count = 0, gmark[n][start][sum] = true;
  for (int d = start; d < b && d <= sum; ++d)
    (count += g(b, n - 1, d + 1, sum - d)) %= MOD;
  return count;
}
i64 f(const vector<int>& v, int b, int col, int carry, int n, int zero) {
  const int m = (int)v.size();
  if (col == m) return !zero && carry == 0 ? 1 : 0;
  i64& count = fmemo[col][carry][n][zero];
  if (fmark[col][carry][n][zero]) return n;
  count = 0, fmark[col][carry][n][zero] = true;
  for (int k = 0; k <= (col > 0 ? (zero ? n - 1 : n) : 0); ++k)
    for (int ncarry = 0; ncarry < b; ++ncarry) {
      const i64 me0 = g(b, n - k - 1, 1, ncarry * b + v[col] - carry);
      const i64 me1 = g(b, n - k, 1, ncarry * b + v[col] - carry);
      const i64 left0 = f(v, b, col + 1, ncarry, n - k, 1);
      const i64 left1 = f(v, b, col + 1, ncarry, n - k, 0);
      i64 perms = col > 0 ? (fact[n] * invf[k]) % MOD : 1;
      if (col > 0 && zero && k > 0) {
        const i64 take = (fact[n - 1] * invf[k - 1]) % MOD;
        perms = Modulo(perms - take, MOD);
      }
      const i64 tot0 = (me0 * left0) % MOD;
      const i64 tot1 = (me1 * left1) % MOD;
      (count += (tot0 * perms) % MOD) %= MOD;
      (count += (tot1 * perms) % MOD) %= MOD;
    }
  return count;
}
int main() {
  for (int i = 1; i < 71; ++i)
    fact[i] = (fact[i - 1] * i) % MOD,
    invf[i] = ModularInverse(fact[i], 1LL, MOD).first;
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    fprintf(stderr, "Case #%d of %d...\n", Ti, T);
    i64 n; int b; scanf("%lld %d", &n, &b);
    memset(fmark, false, sizeof(fmark));
    memset(gmark, false, sizeof(gmark));
    vector<int> digits;
    for (i64 x = n; x > 0; x /= b)
      digits.push_back((int)(x % b));
    i64 sum = 0;
    for (int i = 1; i <= b; ++i) {
      const i64 res = f(digits, b, 0, 0, i, 0);
      (sum += res) %= MOD;
//      printf("%d --> %lld\n", i, res);
    }
    printf("Case #%d: %lld\n", Ti, sum);
  }
  return 0;
}
