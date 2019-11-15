#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>
#include <set>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <fstream>
#include <numeric>
#include <limits.h>

using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef vector<string> vs;
typedef pair<int,int> ii;

#define ITE(v) typeof(v.begin())
#define FOR(i,n) for(int i = 0; i < n; i++)
#define FORIT(it,v) for(ITE(v) it = v.begin(); it != v.end(); it++)
#define ALL(v) v.begin(), v.end()
#define SZ(v) int(v.size())
#define pb push_back
#define SQR(a) ((a)*(a))
#define OK(y,x) (y >= 0 && y < n && x >= 0 && x < m)

#define INF 0x3f3f3f3f
#define EPS (1e-8)

inline int cmp(double a, double b = 0.0) {
  if (fabs(a-b) <= EPS) return 0;
  if (a < b) return -1;
  return 1;
}


const double pi = acos(-1);


struct point {
  double x,y;
  point(double x = 0, double y = 0): x(x), y(y) {}

  point operator +(point q){ return point(x + q.x, y + q.y); }
  point operator -(point q){ return point(x - q.x, y - q.y); }
  point operator *(double t){ return point(x * t, y * t); }
  point operator /(double t){ return point(x / t, y / t); }
  double operator *(point q){ return x * q.x + y * q.y; }
  double operator %(point q){ return x * q.y - y * q.x; }

  int cmp(point q) const {
    if (int t = ::cmp(x, q.x)) return t;
    return ::cmp(y, q.y);
  }
  bool operator ==(point q) const { return cmp(q) == 0; }
  bool operator !=(point q) const { return cmp(q) != 0; }
  bool operator < (point q) const { return cmp(q) < 0; }
};

typedef point vetor;
typedef pair<point,double> circle;
double norma(vetor v){ return hypot(v.x, v.y); }

int main() {
  int ntests;
  scanf("%d",&ntests);
  FOR(kk,ntests) {
    printf("Case #%d: ",kk+1);
    int n,m;
    cin >> n >> m;
    point v[2];
    cin >> v[0].x >> v[0].y;
    cin >> v[1].x >> v[1].y;
    point a;
    FOR(i,m) {
      cin >> a.x >> a.y;
      double r0 = norma(a-v[0]);
      double r1 = norma(a-v[1]);
      double c = norma(v[1]-v[0]);
      // if (cmp(r0) == 0 || cmp(r1) == 0 || cmp(c,r0+r1) <= 0) {
      //   cout << 0 << " ";
      //   continue;
      // }
      // if (v[0] == v[1]) {
      //   cout << 2*pi*r0 << " ";
      //   continue;
      // }
      double cosCBA = (r1*r1 + c*c - r0*r0)/(2*r1*c);
      double CBA = acos(cosCBA);
      double CBD = 2*CBA;
      double cosCAB = (r0*r0 + c*c - r1*r1)/(2*r0*c);
      double CAB = acos(cosCAB);
      double CAD = 2*CAB;
      double AREA = CBD*r1*r1/2. - r1*r1*sin(CBD)/2. + CAD*r0*r0/2 - r0*r0*sin(CAD)/2.;
      printf("%.7lf ",AREA);
    }    
    cout << endl;
  }  
  return 0;
}
