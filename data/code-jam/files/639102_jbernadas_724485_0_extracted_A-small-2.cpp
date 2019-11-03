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
//////////////////////////////////////////////////////////////////////////////////////////
template <class T>
T power(T b, int p) {
  T x(1), y(b);
  for (; p > 0; p /= 2) {
    if (p % 2 == 1) x *= y;
    y *= y;
  }
  return x;
}
//////////////////////////////////////////////////////////////////////////////////////////
struct prime_t {
  static const int MAX_N = 1000001, MAX_P = 1000000;
  bool mark[MAX_N]; int divisor[MAX_N], list[MAX_P], p;
};
void GeneratePrimes(prime_t& primes, int n) {
  memset(primes.mark, true, sizeof(primes.mark)); primes.p = 0;
  primes.mark[1] = false; primes.list[primes.p++] = primes.divisor[2] = 2;
  for (int i = 4; i < n; i += 2) primes.mark[i] = false;
  for (int i = 3; i < n; i += 2) if (primes.mark[i]) {
    primes.divisor[i] = i;
    if (i < (n + i - 1) / i) for (int j = i * i; j < n; j += 2 * i)
      primes.divisor[j] = i, primes.mark[j] = false;
    if (primes.p < prime_t::MAX_P) primes.list[primes.p++] = i;
  }
}
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
bool AllEqual(const vector<int>& v) {
  bool ok = true;
  for (int i = 1; i < (int)v.size() && ok; ++i)
    ok = v[i] == v[0];
  return ok;
}
const i64 GetA(const vector<int>& v, int p) {
  set<i64> sa;
  for (int i = 0; i < (int)v.size() - 2 && (int)sa.size() < 2; ++i) {
    const i64 s21 = Modulo(v[i + 2] - v[i + 1], p);
    const i64 s10 = Modulo(v[i + 1] - v[i + 0], p);
    if (s21 == 0 && s10 == 0) continue;
    if (s10 == 0) return -1;
    const i64 is10 = ModularInverse<i64>(s10, 1LL, p).first;
    sa.insert((s21 * is10) % p);
  }
  if ((int)sa.size() != 1) return -1;
  else return *sa.begin();
}
const i64 GetB(const vector<int>& v, i64 a, int p) {
  if (a == -1) return -1;
  set<i64> sb;
  for (int i = 0; i < (int)v.size() - 2 && (int)sb.size() < 2; ++i) {
    sb.insert(Modulo<i64>(v[i + 1] - (a * v[i + 0]) % p, p));
    sb.insert(Modulo<i64>(v[i + 2] - (a * v[i + 1]) % p, p));
  }
  if ((int)sb.size() != 1) return -1;
  else return *sb.begin();
}
int main() {
  static prime_t primes;
  GeneratePrimes(primes, prime_t::MAX_N);
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    fprintf(stderr, "Case #%d of %d...\n", Ti, T);
    int d, k; scanf("%d %d", &d, &k);
    vector<int> v(k);
    for (int i = 0; i < k; ++i) scanf("%d", &v[i]);
    if (k <= 1) printf("Case #%d: %s\n", Ti, "I don't know.");
    else if (AllEqual(v)) printf("Case #%d: %d\n", Ti, v.back());
    else if (k <= 2) printf("Case #%d: %s\n", Ti, "I don't know.");
    else {
      const int maxVal = *max_element(v.begin(), v.end());
      set<i64> next; bool ambiguous = false;
      const int limit = power(10, d);
      for (int i = 0; i < primes.p && primes.list[i] <= limit && (int)next.size() < 2 && !ambiguous; ++i) {
        const int p = primes.list[i];
        if (p > maxVal) {
          const i64 a = GetA(v, p), b = GetB(v, a, p);
          if (a != -1 && b != -1)
            next.insert(((a * v.back()) % p + b) % p);
        }
      }
      if ((int)next.size() == 1) printf("Case #%d: %lld\n", Ti, *next.begin());
      else printf("Case #%d: %s\n", Ti, "I don't know.");
    }
  }
  return 0;
}
