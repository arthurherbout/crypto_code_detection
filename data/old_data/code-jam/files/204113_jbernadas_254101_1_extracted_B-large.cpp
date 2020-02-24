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
string grid[100];
int minCost[100][100][2][101][2];
int m, n, f;

struct state_t {
  state_t(int i_, int j_, int dir_, int dig_, int other_, int cost_)
    : i(i_), j(j_), dir(dir_), dig(dig_), other(other_), cost(cost_) {}
  bool operator <(const state_t& s) const {
    return s.cost < cost;
  }
  int i, j, dir, dig, other, cost;
};

void AddToFringe(priority_queue<state_t>& fringe, int i, int j,
                 int dir, int dig, int other, int cost) {
  dig = max(dig, 0);
  if (i >= 0 && i < m && j >= 0 && j < n && grid[i][j] != '#') {
    int fall = 0;
    for (; i + 1 < m && grid[i + 1][j] == '.'; ++i, ++fall);
    if (fall <= f) {
      if (fall > 1) dig = 0, other = 0;
      if (cost < minCost[i][j][dir][dig][other]) {
        minCost[i][j][dir][dig][other] = cost;
        fringe.push(state_t(i, j, dir, dig, other, cost));
      }
    }
  }
}
int Dijkstra(int si, int sj) {
  memset(minCost, INFINITE, sizeof(minCost));
  priority_queue<state_t> fringe;
  for (AddToFringe(fringe, si, sj, 0, 0, 0, 0); !fringe.empty();) {
    const state_t s = fringe.top(); fringe.pop();
    if (s.cost > minCost[s.i][s.j][s.dir][s.dig][s.other]) continue;
    if (s.i >= m - 1) return s.cost;
    const string oldRow = grid[s.i];
    if (s.other) {
      const int nj = s.j - dj[s.dir];
      if (nj >= 0 && nj < n) grid[s.i][nj] = '.';
    }
    for (int d = 0; d < s.dig; ++d) {
      const int nj = s.j + d * dj[s.dir];
      if (nj >= 0 && nj < n) grid[s.i][nj] = '.';
    }
    for (int k = 0; k < 2; ++k) {
      const int nother = s.other || (s.dir == k && s.dig > 0) ? 1 : 0;
      AddToFringe(fringe, s.i, s.j + dj[k], s.dir, s.dig - 1, nother, s.cost);
      const string bakRow = grid[s.i + 1];
      bool ok = true;
      for (int a = 1; a <= n && ok; ++a) {
        const int aj = s.j + a * dj[k]; ok = false;
        if (aj >= 0 && aj < n && grid[s.i][aj] == '.' && grid[s.i + 1][aj] == '#') {
          const int nj = aj - dj[k];
          if (nj >= 0 && nj < n && grid[s.i][nj] == '.' && grid[s.i + 1][nj] == '#') {
            grid[s.i + 1][nj] = '.';
            AddToFringe(fringe, s.i, nj, 1 - k, a, 0, s.cost + a), ok = true;
          }
        }
      }
      grid[s.i + 1] = bakRow;
    }
    grid[s.i] = oldRow;
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
    const int res = Dijkstra(0, 0);
    if (res == -1) printf("Case #%d: No\n", Ti);
    else printf("Case #%d: Yes %d\n", Ti, res);
  }
  return 0;
}
