#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

const int N = 2010;

int n, r, g;
long double P[N];

long double profit[N];
long double a[42][42], b[42][42];

long double get_ans(vector <int> reroll) {
//  for (int i = 0; i <= r; i++) {
//    cerr << " " << reroll[i];
//  }
//  cerr << endl;
  for (int i = 0; i <= r; i++) {
    for (int j = 0; j <= r; j++) {
      a[i][j] = 0.0;
    }
  }
  long double q = 1.0 / n;
  for (int i = 0; i <= r; i++) {
    int ni = i + (i < r ? 1 : 0);
    profit[i] = 0;
    for (int j = 0; j < n; j++) {
      if (j >= reroll[i]) {
        profit[i] += P[j];
        a[i][ni] += q;
      } else {
        profit[i] += profit[i - g];
        for (int j = 0; j <= r; j++) {
          a[i][j] += a[i - g][j] * q;
        }
      }
    }
    profit[i] /= n;
  }
//  cerr << "a:" << endl;
//  for (int i = 0; i <= r; i++) {
//    for (int j = 0; j <= r; j++) {
//      cerr << a[i][j] << " ";
//    }
//    cerr << endl;
//  }
  for (int it = 0; it < 50; it++) {
    for (int i = 0; i <= r; i++) {
      long double sum = 0.0;
      for (int j = 0; j <= r; j++) {
        b[i][j] = 0.0;
        for (int k = 0; k <= r; k++) {
          b[i][j] += a[i][k] * a[k][j];
        }
        sum += b[i][j];
      }
      for (int j = 0; j < n; j++) {
        b[i][j] /= sum;
      }
    }
    for (int i = 0; i <= r; i++) {
      for (int j = 0; j <= r; j++) {
        a[i][j] = b[i][j];
      }
    }
  }
//  cerr << "b:" << endl;
//  for (int i = 0; i <= r; i++) {
//    for (int j = 0; j <= r; j++) {
//      cerr << b[i][j] << " ";
//    }
//    cerr << endl;
//  }
  long double res = 0.0;
  for (int j = 0; j <= r; j++) {
    res += profit[j] * a[r][j];
  }
  if (!(res < 100)) {
    cerr << "fault" << endl;
  }
//  cerr << "   res = " << (double)res << endl;
  return res;
}

int main() {
  freopen("in", "r", stdin);
  freopen("out", "w", stdout);
  int tt;
  scanf("%d", &tt);
  for (int qq = 1; qq <= tt; qq++) {
    printf("Case #%d: ", qq);
    fflush(stdout);
    scanf("%d %d %d", &n, &r, &g);
    r *= g;
    for (int i = 0; i < n; i++) {
      double foo;
      scanf("%lf", &foo);
      P[i] = foo;
    }
    sort(P, P + n);
    vector <int> reroll(r + 1);
    for (int i = 0; i <= r; i++) {
      reroll[i] = 0;
    }
    for (int pos = r; pos >= g; pos--) {
      long double cur = get_ans(reroll);
      while (true) {
        if (reroll[pos] == n) {
          break;
        }
        reroll[pos]++;
        long double other = get_ans(reroll);
        if (other < cur) {
          reroll[pos]--;
          break;
        }
        cur = other;
      }
    }
    printf("%.17lf\n", (double)get_ans(reroll));
    fflush(stdout);
    fprintf(stderr, "test %d solved\n", qq);
  }
  return 0;
}
