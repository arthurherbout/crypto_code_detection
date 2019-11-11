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
using namespace std;

#ifndef ONLINE_JUDGE
#pragma warning(disable:4786)  // long identifiers
#pragma warning(disable:4996)  // deprecations
#endif

typedef long long i64;
#define all(x) (x).begin(), (x).end()

//////////////////////////////////////////////////////////////////////////////////////////
const double eps = 5e-8;
int fcomp(double a, double b) {
  if (fabs(a - b) < eps) return 0;
  if (b * (1.0 - eps) < a && a < b * (1.0 + eps)) return 0;
  if (b * (1.0 + eps) < a && a < b * (1.0 - eps)) return 0;
  return a < b ? -1 : +1;
}
//////////////////////////////////////////////////////////////////////////////////////////
bool Inside(double R, double x, double y) {
  return x * x + y * y <= R * R;
}
double FindCut(double x1, double x2, double y, double R) {
  while (fcomp(x1, x2) < 0) {
    const double mx = (x1 + x2) / 2.0;
    const double d2 = mx * mx + y * y;
    (d2 <= R * R ? x1 : x2) = mx;
  }
  return (x1 + x2) / 2.0;
}
double IntegrateCircle(double x, double R) {
  const double cat = sqrt(max(R * R - x * x, 0.0));
  return (x * cat + R * R * atan2(x, cat)) / 2.0;
}
double IntegrateCircle(double x1, double x2, double y, double R) {
  const double start = IntegrateCircle(x1, R);
  const double end = IntegrateCircle(x2, R);
  return (end - start) - (x2 - x1) * y;
}
double ArcArea(double R, double x1, double y1, double x2, double y2) {
  if (Inside(R, x2, y2)) return (x2 - x1) * (y2 - y1);
  if (!Inside(R, x1, y1)) return 0.0;
  if (!Inside(R, x2, y1) && !Inside(R, x1, y2)) {
    const double cx = FindCut(x1, x2, y1, R);
    return IntegrateCircle(x1, cx, y1, R);
  } else if (Inside(R, x2, y1) && Inside(R, x1, y2)) {
    const double cx = FindCut(x1, x2, y2, R);
    const double square = (cx - x1) * (y2 - y1);
    return square + IntegrateCircle(cx, x2, y1, R);
  } else if (Inside(R, x2, y1) && !Inside(R, x1, y2)) {
    return IntegrateCircle(x1, x2, y1, R);
  } else {
    fprintf(stderr, "ERROR\n");
    return 0.0;
  }
}
double GetCoordinate(double r, double g, int i) {
  return r + (g + 2.0 * r) * i;
}
double HitChance(double R, double t, double r, double g) {
  if (g <= 0.0) return 1.0;
  const double totalArea = (acos(0.0) / 2.0) * R * R;
  double gapArea = 0.0;
  for (int i = 0;; ++i) {
    const double x1 = GetCoordinate(r, g, i), x2 = x1 + g;
    const double y1 = GetCoordinate(r, g, i), y2 = y1 + g;
    if (sqrt(x1 * x1) > R - t) break;
    gapArea += ArcArea(R - t, x1, y1, x2, y2);
    for (int j = i - 1; j >= 0; --j) {
      const double x1 = GetCoordinate(r, g, j), x2 = x1 + g;
      const double y1 = GetCoordinate(r, g, i), y2 = y1 + g;
      gapArea += 2.0 * ArcArea(R - t, x1, y1, x2, y2);
    }
  }
  return 1.0 - gapArea / totalArea;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("data.in", "r", stdin);
  freopen("data.out", "w", stdout);
#endif
  srand((unsigned)time(NULL));
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    double f, R, t, r, g;
    scanf("%lf %lf %lf %lf %lf", &f, &R, &t, &r, &g);
    const double res = HitChance(R, t + f, r + f, g - 2.0 * f);
    printf("Case #%d: %.10lf\n", Ti, res);
  }
  return 0;
}
