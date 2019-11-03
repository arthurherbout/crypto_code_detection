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

bool f(int a, int b) {
  if (a <= 0 || b <= 0) return false;
  if (a < b) swap(a, b);
  if (a == b) return false;
  if (a % b == 0) return true;
  return !f(b, a % b) || (a % b + b < a && !f(a % b + b, b));
}
int main() {
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    int a1, a2, b1, b2; scanf("%d %d %d %d", &a1, &a2, &b1, &b2);
    int count = 0;
    for (int a = a1; a <= a2; ++a) for (int b = b1; b <= b2; ++b)
      if (f(a, b)) ++count;
    printf("Case #%d: %d\n", Ti, count);
  }
  return 0;
}
