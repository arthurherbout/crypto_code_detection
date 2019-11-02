#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
#include <vector>
#include <bitset>
#include <cmath>
#pragma comment(linker, "/STACK:256000000")

using namespace std;

const double eps = 1e-9;

struct point {
  double x, y;
  point() { x = y = 0;}
  point(double x, double y) : x(x), y(y) {}
  void read() {
    scanf("%lf%lf", &x, &y);
  }
};

double dist(const point& a, const point& b) {
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double is_in(point& a, point& b, point& c) {
  return fabs(dist(a, b) - dist(a, c) - dist(b, c)) < eps;
}

void make_line(point a, point b, double& A, double& B, double& C) {
  A = a.y - b.y;
  B = b.x - a.x;
  C = a.x * b.y - a.y * b.x;
}

int intersect(point a, point b, point c, point d) {
  double A1, B1, C1;
  double A2, B2, C2;
  make_line(a, b, A1, B1, C1);
  make_line(c, d, A2, B2, C2);

  double dd = (A1 * B2 - A2 * B1);
  double d1 = -C1 * B2 + B1 * C2;
  double d2 = -A1 * C2 + A2 * C1;

  if (fabs(dd) < eps) {
    if ((is_in(a, b, c) && is_in(a, b, d)) || (is_in(c, d, a) && is_in(c, d, b))) {
      return 2;
    }
    return 0;
  }
  point cur(d1 / dd, d2 / dd);
  //bool buf = is_in(c, d, cur);
  if (is_in(a, b, cur) && is_in(c, d, cur)) {
    return 1;
  }
  return 0;
}

bool cmp (point a, point b) {
	return a.x < b.x || a.x == b.x && a.y < b.y;
}

bool cw (point a, point b, point c) {
	return a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y) < 0;
}

bool ccw (point a, point b, point c) {
	return a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y) > 0;
}

void convex_hull (vector<point> & a) {
	if (a.size() == 1)  return;
	sort (a.begin(), a.end(), &cmp);
	point p1 = a[0],  p2 = a.back();
	vector<point> up, down;
	up.push_back (p1);
	down.push_back (p1);
	for (size_t i=1; i<a.size(); ++i) {
		if (i==a.size()-1 || cw (p1, a[i], p2)) {
			while (up.size()>=2 && !cw (up[up.size()-2], up[up.size()-1], a[i]))
				up.pop_back();
			up.push_back (a[i]);
		}
		if (i==a.size()-1 || ccw (p1, a[i], p2)) {
			while (down.size()>=2 && !ccw (down[down.size()-2], down[down.size()-1], a[i]))
				down.pop_back();
			down.push_back (a[i]);
		}
	}
	a.clear();
	for (size_t i=0; i<up.size(); ++i)
		a.push_back (up[i]);
	for (size_t i=down.size()-2; i>0; --i)
		a.push_back (down[i]);
}

double area(vector <point> & cur) {
  double res = 0;
  cur.push_back(cur[0]);
  for (int i = 0; i + 1 < cur.size(); ++i) {
    res += cur[i].x * cur[i + 1].y - cur[i].y * cur[i + 1].x;
  }
  res = fabs(res);
  cur.pop_back();
  return res;
}

bool check(vector <point> p) {
  p.push_back(p[0]);
  p.push_back(p[1]);
  for (int i = 0; i + 2 < p.size(); ++i) {
    if (intersect(p[i], p[i + 1], p[i + 1], p[i + 2]) == 2) {
      return false;
    }
  }

  p.pop_back();

  for (int i = 0; i + 1 < p.size(); ++i) {
    for (int j = i + 2; j + 1 < p.size(); ++j) {
      if (i == 0 && j + 2 == p.size()) continue;
      if (intersect(p[i], p[i + 1], p[j], p[j + 1])) {
        //int res = intersect(p[i], p[i + 1], p[j], p[j + 1]);
        return false;
      }
    }
  }
  return true;
}

void solve(int tt) {
  printf("Case #%d: ", tt + 1);
  int n;
  scanf("%d", &n);
  vector <point> cur(n);
  for (int i = 0; i < n; ++i) {
    cur[i].read();
  }

  vector <point> buf = cur;
  convex_hull(buf);
  double val = area(buf);

  vector <int> order(n);
  for (int i = 0; i < n; ++i) order[i] = i;
  do {
    vector <point> tk(n);
    for (int i = 0; i < n; ++i) {
      tk[i] = cur[order[i]];
    }
    double ar = area(tk);
    if (ar < val / 2 + eps) {
      continue;
    }
    if (check(tk)) {
      for (int i = 0; i < n; ++i) {
        printf("%d ", order[i]);
      }
      printf("\n");
      return;
    }
  } while (next_permutation(order.begin(), order.end()));
}

int main() {
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  int t;
  cin >> t;
  for (int i = 0; i < t; ++i) {
    solve(i);
    cerr << i << endl;
  }

  return 0;
}
