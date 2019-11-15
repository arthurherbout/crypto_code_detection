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

struct state_t {
  state_t(const vector<pair<int, int> >& boxes_, int cost_)
    : boxes(boxes_), cost(cost_) {}
  vector<pair<int, int> > boxes; int cost;
};

const int INFINITE = 0x3f3f3f3f;
const int di[] = {0, -1, 0, +1};
const int dj[] = {-1, 0, +1, 0};
char grid[15][15], dest[15][15];
bool mark[15][15];
int minCost[15][15][15][15];
int m, n;

void AddToFringe(queue<state_t>& fringe, const vector<pair<int, int> >& boxes, int cost) {
  const int i1 = boxes[0].first, j1 = boxes[0].second;
  const int i2 = (int)boxes.size() > 1 ? boxes[1].first : m;
  const int j2 = (int)boxes.size() > 1 ? boxes[1].second : n;
  if (cost < minCost[i1][j1][i2][j2]) {
    minCost[i1][j1][i2][j2] = cost;
    fringe.push(state_t(boxes, cost));
  }
}
bool IsDest() {
  bool ok = true;
  for (int i = 0; i < m && ok; ++i)
    ok = strcmp(grid[i], dest[i]) == 0;
  return ok;
}
bool IsEmpty(int i, int j) {
  return i >= 0 && i < m && j >= 0 && j < n && grid[i][j] == '.';
}
void MarkGroup(int i, int j) {
  if (i >= 0 && i < m && j >= 0 && j < n && grid[i][j] == 'x' && !mark[i][j]) {
    mark[i][j] = true;
    for (int o = 0; o < 4; ++o)
      MarkGroup(i + di[o], j + dj[o]);
  }
}
int CountCurrentGroups() {
  memset(mark, false, sizeof(mark));
  int groups = 0;
  for (int i = 0; i < m && groups <= 2; ++i)
    for (int j = 0; j < n && groups <= 2; ++j)
      if (grid[i][j] == 'x' && !mark[i][j])
        MarkGroup(i, j), ++groups;
  return groups;
}
int bfs(const vector<pair<int, int> >& start) {
  memset(minCost, INFINITE, sizeof(minCost));
  queue<state_t> fringe;
  for (AddToFringe(fringe, start, 0); !fringe.empty();) {
    const state_t s = fringe.front(); fringe.pop();
    for (int i = 0; i < (int)s.boxes.size(); ++i)
      grid[s.boxes[i].first][s.boxes[i].second] = 'x';
    if (IsDest()) return s.cost;
    const bool stateStable = CountCurrentGroups() == 1;
    for (int k = 0; k < (int)s.boxes.size(); ++k) {
      const int i = s.boxes[k].first, j = s.boxes[k].second;
      for (int o = 0; o < 4; ++o) {
        const int ni = i + di[o], nj = j + dj[o];
        const int oi = i + di[(o + 2) % 4], oj = j + dj[(o + 2) % 4];
        if (IsEmpty(ni, nj) && IsEmpty(oi, oj)) {
          grid[i][j] = '.', grid[ni][nj] = 'x';
          const int groups = CountCurrentGroups();
          if (stateStable || groups == 1) {
            vector<pair<int, int> > nboxes(s.boxes);
            nboxes[k] = make_pair(ni, nj);
            sort(nboxes.begin(), nboxes.end());
            AddToFringe(fringe, nboxes, s.cost + 1);
          }
          grid[i][j] = 'x', grid[ni][nj] = '.';
        }
      }
    }
    for (int i = 0; i < (int)s.boxes.size(); ++i)
      grid[s.boxes[i].first][s.boxes[i].second] = '.';
  }
  return -1;
}
int main() {
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    scanf("%d %d", &m, &n);
    for (int i = 0; i < m; ++i) scanf("%s", grid[i]);
    vector<pair<int, int> > start;
    for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j)
      if (grid[i][j] == 'o') start.push_back(make_pair(i, j)), grid[i][j] = '.';
      else if (grid[i][j] == 'w') start.push_back(make_pair(i, j)), grid[i][j] = 'x';
    for (int i = 0; i < m; ++i) strcpy(dest[i], grid[i]);
    for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j)
      if (grid[i][j] == 'x') grid[i][j] = '.';
    printf("Case #%d: %d\n", Ti, bfs(start));
  }
  return 0;
}
