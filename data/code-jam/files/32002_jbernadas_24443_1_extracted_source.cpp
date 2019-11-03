#include "assert.h"
#include "ctype.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include "algorithm"
#include "numeric"
#include "functional"
#include "bitset"
#include "vector"
#include "list"
#include "set"
#include "map"
#include "queue"
#include "stack"
#include "string"
#include "sstream"
#include "iostream"
using namespace std;

#ifndef ONLINE_JUDGE
#pragma warning(disable:4786)  // long identifiers
#pragma warning(disable:4996)  // deprecations
#endif

typedef long long i64;

//////////////////////////////////////////////////////////////////////////////////////////

struct portal_t {
  portal_t(int i_, int j_, int di_, int dj_, int or_)
    : i(i_), j(j_), di(di_), dj(dj_), or(or_) {}
  int i, j, di, dj,or;
};
struct state_t {
  state_t() {}
  state_t(int i_, int j_, int p1_, int p2_, int cost_)
    : i(i_), j(j_), p1(p1_), p2(p2_), cost(cost_) {}
  int i, j, p1, p2, cost;
};

enum {HORZ = 0, VERT = 1};
const int INFINITE = 0x3f3f3f3f;
const int di[] = {-1, +1, 0, 0};
const int dj[] = {0, 0, -1, +1};
const int dpi[] = {0, +1, 0, 0};
const int dpj[] = {0, 0, 0, +1};
const int dor[] = {VERT, VERT, HORZ, HORZ};

int minCost[15][15][481][481];
bool mark[15][15];
int newPortal[15][15][4];
vector<portal_t> portals;
char grid[15][16];
int m, n, p;

pair<int, int> FindAndErase(char c, char o) {
  for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j)
    if (grid[i][j] == c)  {
      grid[i][j] = o;
      return make_pair(i, j);
    }
  assert(false);
  return make_pair(-1, -1);
}

int ShootPortal(int i, int j, int k) {
  while (true) {
    const int ni = i + di[k], nj = j + dj[k];
    if (ni < 0 || ni >= m || nj < 0 || nj >= n || grid[ni][nj] == '#') {
      for (int a = 0; a < p; ++a) {
        const int npi = i + dpi[k], npj = j + dpj[k];
        if (portals[a].or != dor[k] && portals[a].i == npi && portals[a].j == npj)
          return a;
      }
      break;
    } else i = ni, j = nj;
  }
  assert(false);
  return -1;
}

void AddMarkToFringe(queue<pair<int, int> >& fringe, int i, int j) {
  if (i < 0 || i >= m || j < 0 || j >= n) return;
  if (grid[i][j] == '#') return;
  if (!mark[i][j]) {
    fringe.push(make_pair(i, j));
    mark[i][j] = true;
  }
}
bool IsPossible(const pair<int, int>& start, const pair<int, int>& end) {
  memset(mark, false, sizeof(mark));
  queue<pair<int, int> > fringe;
  AddMarkToFringe(fringe, start.first, start.second);
  while (!fringe.empty()) {
    const pair<int, int> s = fringe.front(); fringe.pop();
    if (s == end) return true;
    for (int k = 0; k < 4; ++k) {
      const int ni = s.first + di[k], nj = s.second + dj[k];
      AddMarkToFringe(fringe, ni, nj);
    }
  }
  return false;
}

void AddToFringe(queue<state_t>& fringe, int i, int j, int p1, int p2, int cost) {
  if (i < 0 || i >= m || j < 0 || j >= n) return;
  if (p1 < p && p2 < p && p1 == p2) return;
  if (grid[i][j] == '#') return;
  if (cost < minCost[i][j][p1][p2]) {
    fringe.push(state_t(i, j, p1, p2, cost));
    minCost[i][j][p1][p2] = cost;
  }
}
bool GetNextState(queue<state_t>* fringes, int n, state_t& s) {
  int minFringe = -1;
  s.cost = INFINITE;
  for (int i = 0; i < n; ++i) if (!fringes[i].empty())
    if (fringes[i].front().cost < s.cost)
      s = fringes[minFringe = i].front();
  if (minFringe == -1) return false;
  fringes[minFringe].pop();
  return true;
}
int FindMinPath(const pair<int, int>& start, const pair<int, int>& end) {
  if (!IsPossible(start, end)) return -1;
  memset(minCost, INFINITE, sizeof(minCost));
  queue<state_t> fringe[2];
  AddToFringe(fringe[0], start.first, start.second, p, p, 0);
  for (state_t s; GetNextState(fringe, 2, s);) {
    if (s.i == end.first && s.j == end.second) return s.cost;
    for (int k = 0; k < 4; ++k) {
      const int ni = s.i + di[k], nj = s.j + dj[k];
      AddToFringe(fringe[1], ni, nj, s.p1, s.p2, s.cost + 1);
      if (s.p1 < p && s.p2 < p) {
        for (int p1 = s.p1, p2 = s.p2, i = 0; i < 2; swap(p1, p2), ++i) {
          const int npi = s.i + dpi[k], npj = s.j + dpj[k];
          if (portals[p1].or != dor[k] && portals[p1].i == npi && portals[p1].j == npj) {
            const int ni = portals[p2].di, nj = portals[p2].dj;
            AddToFringe(fringe[1], ni, nj, s.p1, s.p2, s.cost + 1);
          }
        }
      }
    }
    for (int k = 0; k < 4; ++k) {
      const int hit = newPortal[s.i][s.j][k];
      AddToFringe(fringe[0], s.i, s.j, hit, s.p2, s.cost);
      AddToFringe(fringe[0], s.i, s.j, s.p1, hit, s.cost);
    }
  }
  return -1;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("data.in", "r", stdin);
  freopen("data.out", "w", stdout);
#endif
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    scanf("%d %d", &m, &n);
    portals.clear();
    for (int i = 0; i < m; ++i) {
      portals.push_back(portal_t(i, 0, i, 0, VERT));
      portals.push_back(portal_t(i, n, i, n - 1, VERT));
    }
    for (int j = 0; j < n; ++j) {
      portals.push_back(portal_t(0, j, 0, j, HORZ));
      portals.push_back(portal_t(m, j, m - 1, j, HORZ));
    }
    for (int i = 0; i < m; ++i) {
      scanf("%s", &grid[i]);
      for (int j = 0; j < n; ++j) if (grid[i][j] == '#') {
        portals.push_back(portal_t(i, j, i - 1, j, HORZ));
        portals.push_back(portal_t(i, j, i, j - 1, VERT));
        portals.push_back(portal_t(i + 1, j, i + 1, j, HORZ));
        portals.push_back(portal_t(i, j + 1, i, j + 1, VERT));
      }
    }
    const pair<int, int> start = FindAndErase('O', '.');
    const pair<int, int> end = FindAndErase('X', '.');
    p = (int)portals.size();
    for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) if (grid[i][j] == '.')
      for (int k = 0; k < 4; ++k) newPortal[i][j][k] = ShootPortal(i, j, k);
    const int res = FindMinPath(start, end);
    if (res != -1) printf("Case #%d: %d\n", Ti, res);
    else printf("Case #%d: %s\n", Ti, "THE CAKE IS A LIE");
  }
  return 0;
}
