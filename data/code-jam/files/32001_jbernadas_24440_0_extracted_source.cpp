#include "assert.h"
#include "ctype.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include "algorithm"
#include "numeric"
#include "functional"
#include "bitset"
#include "vector"
#include "list"
#include "set"
#include "map"
#include "queue"
#include "stack"
#include "string"
#include "sstream"
#include "iostream"
using namespace std;

#ifndef ONLINE_JUDGE
#pragma warning(disable:4786)  // long identifiers
#pragma warning(disable:4996)  // deprecations
#endif

typedef long long i64;
i64 abs64(i64 x) {return x > 0 ? +x : -x;}

//////////////////////////////////////////////////////////////////////////////////////////
template <class T> struct point_t {
  T x, y; point_t() {}
  point_t(T x_, T y_) : x(x_), y(y_) {}
};
template <class T>
T TriangleArea(const point_t<T>& a, const point_t<T>& b, const point_t<T>& c) {
  // calculates the double of the signed area defined by the points ABC
  return ((c.y - a.y) * (b.x - a.x) - (b.y - a.y) * (c.x - a.x));
}
//////////////////////////////////////////////////////////////////////////////////////////
typedef point_t<i64> point;

vector<point> FindPoints(int N, int M, i64 A) {
  if (N * M < A) return vector<point>();
  vector<point> res;
  for (int x1 = 0; x1 <= N; ++x1) for (int y1 = 0; y1 <= M; ++y1) {
    for (int x2 = 0; x2 <= N; ++x2) for (int y2 = 0; y2 <= M; ++y2) {
      for (int x3 = 0; x3 <= N; ++x3) {
        const i64 num = A + (y2 - y1) * (x3 - x1), den = x2 - x1;
        if (den == 0 || num % den != 0) continue;
        const i64 y3 = num / den + y1;
        if (y3 >= 0 && y3 <= M) {
          const i64 area = TriangleArea(point(x1, y1), point(x2, y2), point(x3, y3));
          res.push_back(point(x1, y1));
          res.push_back(point(x2, y2));
          res.push_back(point(x3, y3));
          return res;
        }
      }
    }
  }
  return res;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("data.in", "r", stdin);
  freopen("data.out", "w", stdout);
#endif
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    int N, M, A; scanf("%d %d %d", &N, &M, &A);
    const vector<point> res = FindPoints(N, M, A);
    if (res.empty()) printf("Case #%d: IMPOSSIBLE\n", Ti);
    else {
      printf("Case #%d:", Ti);
      for (int i = 0; i < (int)res.size(); ++i)
        printf(" %lld %lld", res[i].x, res[i].y);
      printf("\n");
    }
  }
  return 0;
}
