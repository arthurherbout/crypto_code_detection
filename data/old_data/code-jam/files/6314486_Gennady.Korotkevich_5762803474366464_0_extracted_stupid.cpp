#include <bits/stdc++.h>

using namespace std;

long long det(long long a, long long b, long long c,
              long long d, long long e, long long f,
              long long g, long long h, long long i) {
//              fprintf(stderr, "det %lld %lld %lld %lld %lld %lld %lld %lld %lld\n", a, b, c, d, e, f, g, h, i);
  return a * (e * i - f * h) + b * (f * g - d * i) + c * (d * h - e * g);
}

const int N = 77;

int sign[N][N][N];
int x[N], y[N], z[N];

inline int signum(long long x) {
  return (x < 0 ? -1 : (x > 0 ? 1 : 0));
}

inline bool good(long long a, long long b, long long c) {
  return (signum(a) == signum(b) && signum(b) == signum(c));
}

struct V {
  long long x;
  long long y;
  long long z;
};

inline V vect(long long xa, long long ya, long long za, long long xb, long long yb, long long zb) {
  return {ya * zb - za * yb, za * xb - xa * zb, xa * yb - ya * xb};
}

int main() {
  freopen("in", "r", stdin);
  freopen("out", "w", stdout);
  int tt;
  scanf("%d", &tt);
  for (int qq = 1; qq <= tt; qq++) {
    printf("Case #%d: ", qq);
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
      scanf("%d %d %d", x + i, y + i, z + i);
      int g = __gcd(__gcd(abs(x[i]), abs(y[i])), abs(z[i]));
      x[i] /= g;
      y[i] /= g;
      z[i] /= g;
    }
    for (int i = 0; i < n; i++) {
      bool eq = false;
      for (int j = 0; j < i; j++) {
        if (x[i] == x[j] && y[i] == y[j] && z[i] == z[j]) {
          eq = true;
          break;
        }
      }
      if (eq) {
        x[i] = x[n - 1];
        y[i] = y[n - 1];
        z[i] = z[n - 1];
        i--; n--;
      }
    }
    bool inside = false;
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (x[i] == -x[j] && y[i] == -y[j] && z[i] == -z[j]) {
          inside = true;
          break;
        }
      }
      if (inside) break;
    }
    if (!inside) {
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          for (int k = 0; k < n; k++) {
            if (i == j || i == k || j == k) {
              continue;
            }
            long long d = det(-x[i], -y[i], -z[i], x[j] - x[i], y[j] - y[i], z[j] - z[i], x[k] - x[i], y[k] - y[i], z[k] - z[i]);
            long long d1 = det(1-x[i], -y[i], -z[i], x[j] - x[i], y[j] - y[i], z[j] - z[i], x[k] - x[i], y[k] - y[i], z[k] - z[i]);
            long long d2 = det(-x[i], 1-y[i], -z[i], x[j] - x[i], y[j] - y[i], z[j] - z[i], x[k] - x[i], y[k] - y[i], z[k] - z[i]);
            long long d3 = det(-x[i], -y[i], 1-z[i], x[j] - x[i], y[j] - y[i], z[j] - z[i], x[k] - x[i], y[k] - y[i], z[k] - z[i]);
            if (d == 0 && d1 == 0 && d2 == 0 && d3 == 0) {
              sign[i][j][k] = 2;
              continue;
            }
            sign[i][j][k] = (d > 0 ? 1 : (d < 0 ? -1 : 0));
            if (d == 0) {
              V v1 = vect(x[i], y[i], z[i], x[j], y[j], z[j]);
              V v2 = vect(x[j], y[j], z[j], x[k], y[k], z[k]);
              V v3 = vect(x[k], y[k], z[k], x[i], y[i], z[i]);
              if (good(v1.x, v2.x, v3.x) && good(v1.y, v2.y, v3.y) && good(v1.z, v2.z, v3.z)) {
                inside = true;
                break;
              }
            }
          }
          if (inside) break;
        }
        if (inside) break;
      }
      if (!inside) {
        for (int i = 0; i < n; i++) {
          for (int j = i + 1; j < n; j++) {
            for (int k = j + 1; k < n; k++) {
              for (int t = k + 1; t < n; t++) {
                int s0 = sign[i][j][k];
                int s1 = sign[i][k][t];
                int s2 = sign[i][t][j];
                int s3 = sign[j][t][k];
                if (s0 == s1 && s1 == s2 && s2 == s3 && s0 != 2 && s0 != 0) {
                  inside = true;
                  break;
                }
              }
              if (inside) break;
            }
            if (inside) break;
          }
          if (inside) break;
        }
      }
    }
    puts(inside ? "YES" : "NO");
    cerr << "test " << qq << "/" << tt << " solved, time = " << clock() << " ms" << endl;
  }
  return 0;
}
