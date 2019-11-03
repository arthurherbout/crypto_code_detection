#include "cfloat"
#include "cmath"
#include "cstdio"
#include "algorithm"
#include "map"
#include "set"
#include "string"
#include "vector"
using namespace std;
typedef long long i64;

int main() {
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    int n; scanf("%d", &n);
    vector<int> x(n), y(n), r(n);
    for (int i = 0; i < n; ++i)
      scanf("%d %d %d", &x[i], &y[i], &r[i]);
    double best = DBL_MAX;
    if (n == 1) best = r[0];
    else if (n == 2) best = max(r[0], r[1]);
    else {
      vector<int> p(n);
      for (int i = 0; i < n; ++i) p[i] = i;
      do {
        const int i = p[0], j = p[1], k = p[2];
        const int dx = x[i] - x[j], dy = y[i] - y[j];
        const double diameter = sqrt(double(dx * dx + dy * dy)) + r[i] + r[j];
        double R = max<double>(r[k], diameter / 2);
        best = min(best, R);
      } while (next_permutation(p.begin(), p.end()));
    }
    printf("Case #%d: %.10lf\n", Ti, best);
  }
  return 0;
}
