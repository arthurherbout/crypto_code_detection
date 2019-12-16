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

int memo[1000001];
bool mark[1000001];

bool f(int a, int b) {
  bool res = false;
  while (true) {
    if (a < b) swap(a, b);
    if (a == b) return res;
    if (a % b == 0 || a >= 2 * b) return !res;
    a -= b, res = !res;
  }
  return !f(b, a - b);
}
int FindStart(int b) {
  int& res = memo[b];
  if (mark[b]) return res;
  int low = b + 1, high = 2 * b;
  while (low < high) {
    const int mid = (low + high) / 2;
    if (f(mid, b)) high = mid;
    else low = mid + 1;
  }
  return res = low, mark[b] = true;
}
int main() {
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    fprintf(stderr, "Case #%d...\n", Ti);
    int a1, a2, b1, b2; scanf("%d %d %d %d", &a1, &a2, &b1, &b2);
    i64 count = 0;
    for (int a = a1; a <= a2; ++a) {
      const int start = FindStart(a);
      const int c1 = max(start, b1), c2 = b2;
      if (c1 <= c2) count += c2 - c1 + 1;
    }
    for (int b = b1; b <= b2; ++b) {
      const int start = FindStart(b);
      const int c1 = max(start, a1), c2 = a2;
      if (c1 <= c2) count += c2 - c1 + 1;
    }
    printf("Case #%d: %lld\n", Ti, count);
  }
  return 0;
}
