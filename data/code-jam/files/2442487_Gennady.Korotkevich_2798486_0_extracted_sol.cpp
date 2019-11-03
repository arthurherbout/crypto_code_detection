#include <stdio.h>
#include <iostream>

using namespace std;

const int co = (int)1e7;
const long long inf = (long long)1e18;

long long solve(long long p, long long lsh, long long a, long long ll, long long rr, long long n) {
  p -= ll;
  if (p < 0) p += 2*a;
  rr -= ll;
  long long i = 0;
  for (long long it=0;it<=co;it++) {
    if (p <= rr) {
      return i+n;
    }
    long long canadd = (2*a-p)/lsh;
    if (canadd == 0) canadd = 1;
    p += lsh*canadd;
    if (p >= 2*a) p -= 2*a;
    i += canadd;
  }
  return inf;
}

int main() {
  freopen("in", "r", stdin);
  freopen("out", "w", stdout);
  int tt;
  scanf("%d", &tt);
  for (int qq=1;qq<=tt;qq++) {
    printf("Case #%d: ", qq);
    long long a, b, n, m, v, w, y, x, vy, vx;
    cin >> a >> b >> n >> m >> v >> w >> y >> x >> vy >> vx;
    bool plus = true;
    if (vx < 0) vx = -vx, plus = false;
    a *= vx;
    y *= vx;
    vy *= vx;
    v *= vx;
    w *= vx;
    if (vx == 0 || vy == 0) {
      cout << "DRAW" << endl;
      continue;
    }
    long long lfirst, rfirst, shift;
    if (plus) {
      rfirst = (y + (b-x)*(vy/vx)) % (2*a);
      if (rfirst < 0) rfirst += 2*a;
      shift = (2*b*(vy/vx)) % (2*a);
      if (shift < 0) shift += 2*a;
      lfirst = (rfirst + b*(vy/vx)) % (2*a);
      if (lfirst < 0) lfirst += 2*a;
    } else {
      lfirst = (y + x*(vy/vx)) % (2*a);
      if (lfirst < 0) lfirst += 2*a;
      shift = (2*b*(vy/vx)) % (2*a);
      if (shift < 0) shift += 2*a;
      rfirst = (lfirst + b*(vy/vx)) % (2*a);
      if (rfirst < 0) rfirst += 2*a;
    }
    v = (v/vx)*(2*b);
    w = (w/vx)*(2*b);
    long long lshift = (shift*n) % (2*a);
    long long rshift = (shift*m) % (2*a);
    long long lsh = shift, rsh = shift;
    if (lshift > a) {
      lshift = 2*a - lshift;
      lfirst = 2*a - lfirst;
      lsh = 2*a - lsh;
    }
    if (rshift > a) {
      rshift = 2*a - rshift;
      rfirst = 2*a - rfirst;
      rsh = 2*a - rsh;
    }
    long long q1 = inf, q2 = inf;
    if (1.0*v*n < 1e18 && v*n < lshift) {
      long long p = lfirst;
      long long a1 = 0, b1 = a-lshift+(lshift-v*n-1)/2;
      long long a2 = a-(lshift-v*n-1)/2, b2 = a+b1;
      long long a3 = a+a2, b3 = 2*a;
      q1 = solve(p, lsh, a, a1, b1, n);
      long long qq = solve(p, lsh, a, a2, b2, n);
      if (qq < q1) q1 = qq;
      qq = solve(p, lsh, a, a3, b3, n);
      if (qq < q1) q1 = qq;
    }
    if (1.0*w*m < 1e18 && w*m < rshift) {
      long long p = rfirst;
      long long a1 = 0, b1 = a-rshift+(rshift-w*m-1)/2;
      long long a2 = a-(rshift-w*m-1)/2, b2 = a+b1;
      long long a3 = a+a2, b3 = 2*a;
      q2 = solve(p, rsh, a, a1, b1, m);
      long long qq = solve(p, rsh, a, a2, b2, m);
      if (qq < q2) q2 = qq;
      qq = solve(p, rsh, a, a3, b3, m);
      if (qq < q2) q2 = qq;
    }
    if (q1 == inf && q2 == inf) cout << "DRAW" << endl; else
    if (q1 > q2 || q1 == q2 && plus) cout << "LEFT " << q2 << endl;
    else cout << "RIGHT " << q1 << endl;
  }
  return 0;
}
