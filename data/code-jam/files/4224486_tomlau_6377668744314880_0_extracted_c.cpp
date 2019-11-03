#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

int T, i, N;

typedef pair<long long, long long> point;
#define x first
#define y second

int bitcount(int n) {
  int cnt=0;
  for (int i=0; i<20; i++)
    if (n & (1<<i)) cnt++;
  return cnt;
}

int orientation(point p, point q, point r) {
  long long val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
  if (val == 0) return 0;
  return (val > 0) ? 1 : 2;
}
 
bool isOnConvexHullBoundary(vector <point> &points, int mask, int which) {
  int n = points.size();
  int cnt = bitcount(mask);
  if (cnt <= 3) return true;

  int l = -1;
  bool vis[20];
  for (int i=0; i<n; i++) {
    vis[i] = false;
    if (mask & (1<<i))
      if (l == -1 || points[i].x < points[l].x)
        l = i;
  }

  int p = l, q;
  do {
    q = (p+1)%n;
    while (!(mask & (1<<q)))
      q = (q+1)%n;
    for (int i=0; i<n; i++)
      if (mask & (1<<i))
        if (orientation(points[p], points[i], points[q]) == 2)
          q = i;

    if (p == which || q == which || orientation(points[p], points[which], points[q]) == 0)
      return true;

    if (vis[q]) return false;
    vis[q] = true;

    p = q;
  } while (p != l);
  return false;
}


int main() {
  cin >> T;
  for (int t=1; t<=T; t++) {
    int N;
    cin >> N;
    vector<point> pts(N);
    for (int i=0; i<N; i++)
      cin >> pts[i].x >> pts[i].y;

    cout << "Case #" << t << ": " << endl;
    for (int i=0; i<N; i++) {
      if (N <= 3)
        cout << 0 << endl;
      else {
        int best = N-3;
        for (int mask=0; mask<(1<<N); mask++) {
          //cerr << mask << endl;
          if (mask & (1<<i)) {
            int bc = bitcount(mask);
            if (N-bc < best)
              if (isOnConvexHullBoundary(pts, mask, i))
                best = N-bc;
          }
        }
        cout << best << endl;
      }
    }
 }
}
