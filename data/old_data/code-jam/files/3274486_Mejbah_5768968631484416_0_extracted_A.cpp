#include <bits/stdc++.h>

using namespace std;

struct data {
  double r, h;
  bool operator<(const data& a) const {
    if (r == a.r) {
      return h > a.h;
    }
    return r > a.r;
  }
}d;

int n, k;
vector<data> A;
double mem[1003][1003];
bool vis[1003][1003];
const double pi = acos(-1.);

double area(double r) {
  return pi * r * r;
}

double height(double r, double h) {
  return 2. * pi * r * h;
}

double area2(double r1, double r0) {
  return pi * (r1 * r1 - r0 * r0);
}

double solve(int a, int m) {
  if (m == k) {
    return area(A[a].r) + height(A[a].r, A[a].h);
  }
  if (vis[a][m]) {
    return mem[a][m];
  }
  double r0 = 0;
  for (int i = a + 1; i < n; i++) {
    double r = area2(A[a].r, A[i].r) + height(A[a].r, A[a].h) + solve(i, m + 1);
    r0 = max(r, r0);
    /*
    cout << r << ' ' << r0 << endl;
    cout << a << ' ' << A[a].r << endl;
    cout << i << ' ' << A[i].r << endl;
    cout << area2(A[a].r, A[i].r) << ' ' << height(A[a].r, A[a].h) << ' ' << solve(i, m + 1) << endl;
    cout << area2(A[a].r, A[i].r) + height(A[a].r, A[a].h) + solve(i, m + 1) << endl;
    */
  }
  vis[a][m] = 1;
  return mem[a][m] = r0;
}

int main() {
  int t;
  scanf("%d", &t);
  for (int cn = 1; cn <= t; cn++) {
    scanf("%d%d", &n, &k);
    A.resize(n);
    for (int i = 0; i < n; i++) {
      scanf("%lf%lf", &A[i].r, &A[i].h);
    }
    sort(A.begin(), A.end());
    memset(vis, 0, sizeof vis);
    double ans = 0;
    if (k == 1) {
      for (int a = 0; a < n; a++) {
        ans = max(ans, area(A[a].r) + height(A[a].r, A[a].h));
      }
    } else {
      for (int i = 0; i < n; i++) {
        ans = max(ans, solve(i, 1));
      }
    }
    printf("Case #%d: %.10f\n", cn, ans);
    A.clear();
  }
}
