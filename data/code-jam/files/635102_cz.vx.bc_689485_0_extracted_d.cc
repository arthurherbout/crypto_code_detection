#include <iostream>
#include <complex>
#include <algorithm>
using namespace std;
typedef long double ld;
typedef complex<ld> point;
const ld pi = acos(-1.0);
const ld eps = 1e-9;

ld dp(const point &a, const point &b) {
  return a.real()*b.real() + a.imag()*b.imag();
}

ld cp(const point &a, const point &b) {
  return a.real()*b.imag() - a.imag()*b.real();
}

// area of a sector: c - center, u - start point, v - end point;
// sector is sweeped by u-c rotated to v-c clockwise
ld sect(const point &c, const point &u, const point &v) {
  if (cp(v-c, u-c) < 0) return abs(u-c)*abs(u-c)*pi - sect(c, v, u);
  if (cp(v-c, u-c) < eps) return abs(u-c)*abs(u-c)*pi/2;
  ld a = acos(dp(v-c, u-c)/abs(v-c)/abs(u-c));
  return a/2*abs(u-c)*abs(u-c);
}

// area of a segment: c - center, u - start point, v - end point;
// segment is the area inside the shape u ==CW arc==> v ==line==> u
ld seg(const point &c, const point &u, const point &v) {
  return sect(c, u, v) + cp(u-c, v-c)/2;
}

// circle intersection: sets u and v to the two points of intersection,
// which are assumed to exist (CHECK FIRST!!!)
// after the call, cp(u-c1, v-c1) < 0
void isect_cc(const point &c1, ld r1, const point &c2, ld r2, point &u, point &v) {
  ld d = abs(c2-c1);
  ld x = (r1*r1-r2*r2+d*d)/2/d;
  ld y = sqrt(r1*r1-x*x);
  u = c1 + x/d*(c2-c1) + y/d*(c2-c1)*point(0, 1);
  v = c1 + x/d*(c2-c1) - y/d*(c2-c1)*point(0, 1);
}

// disk intersection: returns the area enclosed in both circles
// works in all cases (tested on 11322)
ld isect_cc_a(const point &c1, ld r1, const point &c2, ld r2) {
  if (abs(r1) < eps || abs(r2) < eps || r1+r2 < abs(c2-c1)+eps) return 0;
  if (abs(c2-c1) < abs(r2-r1)+eps) return min(pi*r1*r1, pi*r2*r2);

  point u, v;
  isect_cc(c1, r1, c2, r2, u, v);

  return seg(c1, u, v) + seg(c2, v, u);
}

point P[20], Q[20];

int main() {
  int nt, C = 1;
  cin >> nt;
  int n, m;
  while (nt-- && cin >> n >> m) {
    ld x, y;
    for (int i = 0; i < n && cin >> x >> y; ++i)
      P[i] = point(x, y);
    
    for (int i = 0; i < m && cin >> x >> y; ++i)
      Q[i] = point(x, y);

    cout << "Case #" << C++ << ":";
    for (int i = 0; i < m; ++i) {
      // small case only!
      cout.setf(ios::fixed);
      cout.precision(7);
      cout << " " << isect_cc_a(P[0], abs(P[0]-Q[i]), P[1], abs(P[1]-Q[i]));
    }
    cout << endl;
  }
}
