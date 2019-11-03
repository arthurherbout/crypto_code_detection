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
  state_t(int i_, int x_, int cost_) : i(i_), x(x_), cost(cost_) {}
  bool operator <(const state_t& rhs) const {
    return rhs.cost < cost;
  }
  int i, x, cost;
};
const int INFINITE = 0x3f3f3f3f;
int minCost[102][257];

void AddToFringe(priority_queue<state_t>& fringe, int i, int x, int cost) {
  if (cost < minCost[i][x])
    fringe.push(state_t(i, x, minCost[i][x] = cost));
}
int Dijkstra(const vector<int>& v, int del, int ins, int m) {
  memset(minCost, INFINITE, sizeof(minCost));
  priority_queue<state_t> fringe;
  for (int i = 0; i < (int)v.size(); ++i) {
    AddToFringe(fringe, i + 1, v[i], i * del);
    for (int nx = 0; nx <= 255; ++nx) {
      AddToFringe(fringe, i, nx, i * del + ins);
      AddToFringe(fringe, i + 1, nx, i * del + abs(nx - v[i]));
    }
  }
  while (!fringe.empty()) {
    const state_t s = fringe.top(); fringe.pop();
    if (s.cost > minCost[s.i][s.x]) continue;
    if (s.i >= (int)v.size()) return s.cost;
    if (abs(v[s.i] - s.x) <= m) AddToFringe(fringe, s.i + 1, v[s.i], s.cost);
    AddToFringe(fringe, s.i + 1, s.x, s.cost + del);
    for (int nx = max(s.x - m, 0); nx <= min(s.x + m, 255); ++nx) {
      AddToFringe(fringe, s.i, nx, s.cost + ins);
      AddToFringe(fringe, s.i + 1, nx, s.cost + abs(nx - v[s.i]));
    }
  }
  return -1;
}
int main() {
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    int del, ins, m, n; scanf("%d %d %d %d", &del, &ins, &m, &n);
    vector<int> v(n);
    for (int i = 0; i < n; ++i) scanf("%d", &v[i]);
    printf("Case #%d: %d\n", Ti, Dijkstra(v, del, ins, m));
  }
  return 0;
}
