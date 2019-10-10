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
const int INFINITE = 0x3f3f3f3f;

struct rect_t {
  rect_t(int x1_, int y1_, int x2_, int y2_) : x1(x1_), y1(y1_), x2(x2_), y2(y2_) {}
  int x1, y1, x2, y2;
};
char grid[2][100][101];
int m, n;

rect_t GetBoundingBox(const vector<rect_t>& v) {
  rect_t bbox(INFINITE, INFINITE, -INFINITE, -INFINITE);
  for (int i = 0; i < (int)v.size(); ++i)
    bbox.x1 = min(bbox.x1, v[i].x1), bbox.y1 = min(bbox.y1, v[i].y1),
    bbox.x2 = max(bbox.x2, v[i].x2), bbox.y2 = max(bbox.y2, v[i].y2);
  return bbox;
}
void InitGrid(char grid[][101], int m, int n) {
  for (int i = 0; i < m; ++i) grid[i][n] = '\0';
  for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) grid[i][j] = '.';
}
int CountAll(const char grid[][101], int m, int n, int c) {
  int count = 0;
  for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j)
    if (grid[i][j] == c) ++count;
  return count;
}
int cell(const char grid[][101], int i, int j, int m, int n) {
  if (!(i >= 0 && i < m) || !(j >= 0 && j < n)) return 0;
  else return grid[i][j] == 'X' ? 1 : 0;
}
int RunStep(const char grid[][101], char ngrid[][101], int m, int n) {
  int left = 0;
  InitGrid(ngrid, m, n);
  for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) {
    const int neighbours = cell(grid, i - 1, j, m, n) + cell(grid, i, j - 1, m, n);
    if (grid[i][j] == 'X') ngrid[i][j] = neighbours > 0 ? 'X' : '.';
    if (grid[i][j] == '.') ngrid[i][j] = neighbours < 2 ? '.' : 'X';
    if (ngrid[i][j] == 'X') ++left;
  }
  return left;
}
int main() {
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    fprintf(stderr, "Case #%d of %d...\n", Ti, T);
    int r; scanf("%d", &r);
    vector<rect_t> v;
    for (int i = 0; i < r; ++i) {
      int x1, y1, x2, y2; scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
      v.push_back(rect_t(x1, y1, x2 + 1, y2 + 1));
    }
    const rect_t bbox = GetBoundingBox(v);
    m = bbox.y2 - bbox.y1, n = bbox.x2 - bbox.x1;
    for (int i = 0; i < r; ++i)
      v[i].x1 -= bbox.x1, v[i].x2 -= bbox.x1,
      v[i].y1 -= bbox.y1, v[i].y2 -= bbox.y1;
    InitGrid(grid[0], m, n);
    for (int k = 0; k < r; ++k)
      for (int i = v[k].y1; i < v[k].y2; ++i)
        for (int j = v[k].x1; j < v[k].x2; ++j)
          grid[0][i][j] = 'X';
    int turns = 0;
    for (int left = CountAll(grid[0], m, n, 'X'); left > 0; ++turns) {
      const int me = (turns + 1) % 2, old = turns % 2;
      left = RunStep(grid[old], grid[me], m, n);
    }
    printf("Case #%d: %d\n", Ti, turns);
  }
  return 0;
}
