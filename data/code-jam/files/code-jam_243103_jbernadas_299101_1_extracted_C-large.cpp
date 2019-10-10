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

struct point_t {
  point_t(int x_, int y_)
    : x(x_), y(y_) {}
  bool operator <(const point_t& p) const {return x < p.x;}
  int x, y;
};

int GetNewColor(const vector<int>& colors) {
  for (int i = 0; i < (int)colors.size(); ++i)
    if (i != colors[i]) return i;
  return (int)colors.size();
}
int main() {
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    int n; scanf("%d", &n);
    fprintf(stderr, "Ti=%d n=%d\n", Ti, n);
    vector<point_t> v;
    for (int i = 0; i < n; ++i) {
      int x, y; scanf("%d %d", &x, &y);
      v.push_back(point_t(x, y));
    }
    if (Ti == 70) {
      fprintf(stderr, "%d\n", n);
      for (int i = 0; i < n; ++i)
        fprintf(stderr, "%d %d\n", v[i].x, v[i].y);
    }
    sort(v.begin(), v.end());
    vector<int> ycolor(32, -1);
    int needed = 0;
    for (int i = n - 1; i >= 0; --i) {
      const int y = v[i].y;
      vector<int> colors;
      for (int dy = -1; dy <= +1; ++dy)
        if (ycolor[y + dy] != -1)
          colors.push_back(ycolor[y + dy]);
      sort(colors.begin(), colors.end());
      ycolor[y] = GetNewColor(colors);
      needed = max(needed, ycolor[y] + 1);
    }
    printf("Case #%d: %d\n", Ti, needed);
  }
  return 0;
}
