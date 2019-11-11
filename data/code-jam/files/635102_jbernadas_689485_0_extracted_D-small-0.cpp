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

//////////////////////////////////////////////////////////////////////////////////////////
struct point_t {
  point_t(double x_, double y_) : x(x_), y(y_) {}
  double x, y;
};
struct circle_t {
  circle_t(const point_t& c_, double r_) : c(c_), r(r_) {}
  point_t c; double r;
};
//////////////////////////////////////////////////////////////////////////////////////////
double GetCircleIntersectionX(const circle_t& c1, const circle_t& c2) {
  // calculate the intersection points between both circles
  const double dx = c2.c.x - c1.c.x, dy = c2.c.y - c1.c.y;
  const double d = sqrt(fabs(dx * dx + dy * dy));
  const double a = (c1.r * c1.r - c2.r * c2.r + d * d) / (2.0 * d);
  const double disc = c1.r * c1.r - a * a;
  return c1.c.x + dx * (a / d);
}
//////////////////////////////////////////////////////////////////////////////////////////
circle_t MakeCircle(const point_t& center, const point_t& pivot) {
  const double dx = pivot.x - center.x, dy = pivot.y - center.y;
  const double r = sqrt(dx * dx + dy * dy);
  return circle_t(center, r);
}
double f(double a, const circle_t& c) {
  const double disc = sqrt(c.r * c.r - a * a);
  return 0.5 * a * disc + 0.5 * c.r * c.r * atan2(a, disc);
}
double GetIntersectionArea(const circle_t& c1, const circle_t& c2) {
  const double dx = c2.c.x - c1.c.x, dy = c2.c.y - c1.c.y;
  const double dist = sqrt(dx * dx + dy * dy);
  if (dist >= c1.r + c2.r) return 0.0;
  const double ix = GetCircleIntersectionX(c1, c2);
  const double topLeft = f(c2.r, c2) - f(dist - ix, c2);
  const double topRight = f(c1.r, c1) - f(ix, c1);
  return 2.0 * (topLeft + topRight);
}
double GetArea(const vector<point_t>& v, const point_t& pivot) {
  vector<circle_t> circles;
  for (int i = 0; i < (int)v.size(); ++i)
    circles.push_back(MakeCircle(v[i], pivot));
  const circle_t& c1 = circles[0];
  const circle_t& c2 = circles[1];
  const double dx = c2.c.x - c1.c.x, dy = c2.c.y - c1.c.y;
  const double dist = sqrt(dx * dx + dy * dy);
  const circle_t phantom1(point_t(0, 0), c1.r);
  const circle_t phantom2(point_t(dist, 0), c2.r);
  return GetIntersectionArea(phantom1, phantom2);
}

int main() {
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    fprintf(stderr, "Case #%d of %d...\n", Ti, T);
    int n, m; scanf("%d %d", &n, &m);
    vector<point_t> goats, buckets;
    for (int i = 0; i < n; ++i) {
      double x, y; scanf("%lf %lf", &x, &y);
      goats.push_back(point_t(x, y));
    }
    for (int i = 0; i < m; ++i) {
      double x, y; scanf("%lf %lf", &x, &y);
      buckets.push_back(point_t(x, y));
    }
    vector<double> area(m);
    for (int i = 0; i < m; ++i)
      area[i] = GetArea(goats, buckets[i]);
    printf("Case #%d:", Ti);
    for (int i = 0; i < m; ++i) printf(" %.10lf", area[i]);
    printf("\n");
  }
  return 0;
}
