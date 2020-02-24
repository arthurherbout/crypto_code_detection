#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
  srand(time(0));
  int T, prob=1;
  for (cin >> T; T--;) {
    int N;
    cin >> N;
    vector<double> X(N), Y(N), Z(N);
    for (int i = 0; i < N; i++) {
      cin >> X[i] >> Y[i] >> Z[i];
      double dist = hypot(hypot(X[i], Y[i]), Z[i]);
      X[i] /= dist; Y[i] /= dist; Z[i] /= dist;
    }
    bool ret = true;
    for (int attempts = 0; attempts < 10000; attempts++) {
      int i = rand()%N, j = rand()%N;
      if (i == j) { attempts--; continue; }
      double x1 = X[i] + (rand()%2001-1000) / 1000000.0;
      double y1 = Y[i] + (rand()%2001-1000) / 1000000.0;
      double z1 = Z[i] + (rand()%2001-1000) / 1000000.0;
      double d1 = hypot(hypot(x1, y1), z1);
      x1 /= d1; y1 /= d1; z1 /= d1;
      double x2 = X[j] + (rand()%2001-1000) / 1000000.0;
      double y2 = Y[j] + (rand()%2001-1000) / 1000000.0;
      double z2 = Z[j] + (rand()%2001-1000) / 1000000.0;
      double d2 = hypot(hypot(x2, y2), z2);
      x2 /= d2; y2 /= d2; z2 /= d2;
      double x3 = y1*z2 - z1*y2;
      double y3 = z1*x2 - x1*z2;
      double z3 = x1*y2 - y1*x2;
      double d3 = hypot(hypot(x3, y3), z3);
      if (fabs(d3) < 0.00001) { attempts--; continue; }
      x3 /= d3; y3 /= d3; z3 /= d3;

      bool pos = false, neg = false;
//cout << x3  << ' ' << y3 << ' ' << z3 << "  ";
      for (int i = 0; i < N; i++) {
        double dp = x3*X[i] + y3*Y[i] + z3*Z[i];
//cout << ' ' << dp;
        if (dp <= 1e-9) neg = true;
        if (dp >= -1e-9) pos = true;
      }
//cout << endl;
      if (!neg || !pos) { ret = false; break; }
    }
    cout << "Case #" << prob++ << ": " << (ret ? "YES" : "NO") << endl;
  }
}
