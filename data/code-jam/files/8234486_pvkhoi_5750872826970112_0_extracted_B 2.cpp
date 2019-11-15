#include <bits/stdc++.h>

using namespace std;

const int MAXN = 105;
const double eps = 1e-7;

typedef long long ll;

int n;
double V, X;
double r[MAXN], c[MAXN];
ll iV, iX, ir[MAXN], ic[MAXN];

void solve() {
   if (n == 1) {
      if (iX != ic[0]) {
         puts("IMPOSSIBLE"); return;
      }
      cout << fixed << setprecision(9) << (double)iV / ir[0] << endl;
      return;
   }
   if (iX == ic[0] && iX == ic[1]) {
      cout << fixed << setprecision(9) << (double)iV / (ir[0] + ir[1]) << endl;
      return;
   }
   if (iX == ic[0]) {
      cout << fixed << setprecision(9) << (double)iV / ir[0] << endl;
      return;
   }
   else if (iX == ic[1]) {
      cout << fixed << setprecision(9) << (double)iV / ir[1] << endl;
      return;
   }
   if (ic[0] < iX && ic[1] < iX) {
      puts("IMPOSSIBLE"); return;
   }
   if (ic[0] > iX && ic[1] > iX) {
      puts("IMPOSSIBLE"); return;
   }
   ll a = iX - ic[0], b = iX - ic[1];
   double v0 = (double)-b * iV / (double)(a - b);
   double v1 = iV - v0;
   double res = max(v0 / ir[0], v1 / ir[1]);
   // double kk = v0 / r[0];
//   if (v0 < -eps) {
//      puts("IMPOSSIBLE"); return;
//   }
   cout << fixed << setprecision(9) << res << endl;
}

int main() {
   freopen("input.txt", "r", stdin);
   freopen("output.txt", "w", stdout);

   int n_test; cin >> n_test;
   for (int t = 1; t <= n_test; t++) {
      cin >> n >> V >> X;
      iV = (V + eps) * 10000;
      iX = (X + eps) * 10000;
      for (int i = 0; i < n; i++) {
         cin >> r[i] >> c[i];
         ir[i] = (r[i] + eps) * 10000;
         ic[i] = (c[i] + eps) * 10000;
      }
      printf("Case #%d: ", t);
      solve();
//      if (t == 59) {
//         puts("asjdaskjfkasf");
//         cout << n << ' ' << V << ' ' << X << endl;
//         cout << r[0] << ' ' << c[0] << endl;
//         cout << r[1] << ' ' << c[1] << endl;
//      }
   }

   return 0;
}
