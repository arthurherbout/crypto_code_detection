#include "cmath"
#include "cstdio"
#include "algorithm"
#include "map"
#include "queue"
#include "set"
#include "string"
#include "vector"
using namespace std;
typedef long long i64;

const int INFINITE = 0x3f3f3f3f;
const int dj[] = {-1, +1};
string grid[10];
int minCost[10][10][1 << 6][1 << 6];
int m, n, f;

struct state_t {
  state_t(int i_, int j_, int m1_, int m2_, int cost_)
    : i(i_), j(j_), m1(m1_), m2(m2_), cost(cost_) {}
  bool operator <(const state_t& s) const {
    return s.cost < cost;
  }
  int i, j, m1, m2, cost;
};

int GetMask(int i) {
  int mask = 0;
  if (i < m)
    for (int j = 0; j < n; ++j)
      if (grid[i][j] == '.')
        mask |= 1 << j;
  return mask;
}
void CopyMask(int i, int mask) {
  if (i < m)
    for (int j = 0; j < n; ++j)
      grid[i][j] = (mask & (1 << j)) ? '.' : '#';
}
void AddToFringe(priority_queue<state_t>& fringe, int i, int j, int cost) {
  if (i >= 0 && i < m && j >= 0 && j < n && grid[i][j] != '#') {
    int fall = 0;
    for (; i + 1 < m && grid[i + 1][j] == '.'; ++i, ++fall);
    if (fall <= f) {
      const int m1 = GetMask(i), m2 = GetMask(i + 1);
      if (cost < minCost[i][j][m1][m2]) {
        minCost[i][j][m1][m2] = cost;
        fringe.push(state_t(i, j, m1, m2, cost));
      }
    }
  }
}
int Dijkstra() {
  memset(minCost, INFINITE, sizeof(minCost));
  priority_queue<state_t> fringe;
  for (AddToFringe(fringe, 0, 0, 0); !fringe.empty();) {
    const state_t s = fringe.top(); fringe.pop();
    if (s.cost > minCost[s.i][s.j][s.m1][s.m2]) continue;
    if (s.i >= m - 1) return s.cost;
    const string row1 = grid[s.i], row2 = grid[s.i + 1];
    CopyMask(s.i, s.m1); CopyMask(s.i + 1, s.m2);
    for (int k = 0; k < 2; ++k) {
      const int nj = s.j + dj[k];
      AddToFringe(fringe, s.i, nj, s.cost);
      if (nj >= 0 && nj < n && grid[s.i][nj] == '.' && grid[s.i + 1][nj] == '#') {
        grid[s.i + 1][nj] = '.';
        AddToFringe(fringe, s.i, s.j, s.cost + 1);
        grid[s.i + 1][nj] = '#';
      }
    }
    grid[s.i] = row1, grid[s.i + 1] = row2;
  }
  return -1;
}
int main() {
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    scanf("%d %d %d", &m, &n, &f);
    for (int i = 0; i < m; ++i) {
      char str[100]; scanf("%s", str);
      grid[i] = string(str);
    }
    const int res = Dijkstra();
    if (res == -1) printf("Case #%d: No\n", Ti);
    else printf("Case #%d: Yes %d\n", Ti, res);
  }
  return 0;
}
