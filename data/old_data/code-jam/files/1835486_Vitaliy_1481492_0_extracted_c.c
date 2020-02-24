#include <stdio.h>
#include <assert.h>

#define MAXN 10001
long long x[MAXN], y[MAXN];

long long e;
long long f;
int n;


long long rem (long long e) {
  if (e < f) {
    return 0;
  }
  e -= f;
  int i;
  int lastx = -1;
  long long r = 0;
  for (i = 0; i < n; i++) {
    if (i != 0 && y[i] == y[i - 1]) {
      continue;
    }
    long long z = x[i] > 0 ? e / x[i] : y[i];
    int ok = 0;
    if (z >= y[i] - lastx) {
      z = y[i] - lastx;
      ok = 1;
      lastx = y[i];
    }
    r += z;
    e -= x[i] * z;
    if (!ok) {
      break;
    }
  }
  return r;
}

int main () {
  int __;
  scanf ("%d", &__);
  int _;
  for (_ = 0; _ < __; _ ++) {
    printf ("Case #%d:", _ + 1);
    scanf ("%lld %lld %d", &e, &f, &n);
    int i, j;
    for (i = 0; i < n; i++) {
      scanf ("%lld %lld", &x[i], &y[i]);
      //printf ("%lld\n", x[i]);
    }
    for (i = 0; i < n; i++) {
      for (j = i + 1; j < n; j++) if (y[i] > y[j]) {
        long long t;
        t = x[i]; x[i] = x[j]; x[j] = t;
        t = y[i]; y[i] = y[j]; y[j] = t;
      }
    }
    for (i = n - 1; i >= 1; i --) if (x[i - 1] > x[i]) {
      x[i - 1] = x[i];
    }
    long long best = 0;
    long long lastx = -1;
    long long lastv = f;
    for (i = 0; i < n; i++) {
      if (i != 0 && y[i] == y[i - 1]) {
        continue;
      }
      if (e < f + lastv) {
        break;        
      } else {
        assert (y[i] > lastx);
        long long ee = e;
        long long rz = 0;
        assert (x[i] > 0);
        long long z = x[i] > 0 ? (e - f - lastv) / (x[i]) : y[i];
        int ok = 0;
        if (z >= (y[i] - lastx)) {          
          z = y[i] - lastx;
          long long d = lastv + z * x[i];
          long long h = e / d;
          rz += (y[i] + 1) * h;
          ee -= d * h;
          lastx = y[i];
          lastv = d;
          ok = 1;
        } 
        rz += rem (ee);
        if (rz > best) {
          best = rz;
        }
        if (!ok) { 
          break;
        }
      }
    }
    printf (" %lld", best);
    printf ("\n");
  }
  return 0;
}
