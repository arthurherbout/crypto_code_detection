#include <cmath>
#include <vector>
#include <iostream>
#include <queue>
#include <cstring>

using namespace std;

// const int INF = 2000000000;

// struct Edge {
//   int from, to, cap, flow, index;
//   Edge(int from, int to, int cap, int flow, int index) :
//     from(from), to(to), cap(cap), flow(flow), index(index) {}
// };

// struct Dinic {
//   int N;
//   vector<vector<Edge> > G;
//   vector<Edge *> dad;
//   vector<int> Q;
  
//   Dinic(int N) : N(N), G(N), dad(N), Q(N) {}
  
//   void AddEdge(int from, int to, int cap) {
//     G[from].push_back(Edge(from, to, cap, 0, G[to].size()));
//     if (from == to) G[from].back().index++;
//     G[to].push_back(Edge(to, from, 0, 0, G[from].size() - 1));
//   }

//   long long BlockingFlow(int s, int t) {
//     fill(dad.begin(), dad.end(), (Edge *) NULL);
//     dad[s] = &G[0][0] - 1;
    
//     int head = 0, tail = 0;
//     Q[tail++] = s;
//     while (head < tail) {
//       int x = Q[head++];
//       for (int i = 0; i < G[x].size(); i++) {
//     Edge &e = G[x][i];
//     if (!dad[e.to] && e.cap - e.flow > 0) {
//       dad[e.to] = &G[x][i];
//       Q[tail++] = e.to;
//     }
//       }
//     }
//     if (!dad[t]) return 0;

//     long long totflow = 0;
//     for (int i = 0; i < G[t].size(); i++) {
//       Edge *start = &G[G[t][i].to][G[t][i].index];
//       int amt = INF;
//       for (Edge *e = start; amt && e != dad[s]; e = dad[e->from]) {
//     if (!e) { amt = 0; break; }
//     amt = min(amt, e->cap - e->flow);
//       }
//       if (amt == 0) continue;
//       for (Edge *e = start; amt && e != dad[s]; e = dad[e->from]) {
//     e->flow += amt;
//     G[e->to][e->index].flow -= amt;
//       }
//       totflow += amt;
//     }
//     return totflow;
//   }

//   long long GetMaxFlow(int s, int t) {
//     long long totflow = 0;
//     while (long long flow = BlockingFlow(s, t))
//       totflow += flow;
//     return totflow;
//   }
// };

// Adjacency list implementation of FIFO push relabel maximum flow
// with the gap relabeling heuristic.  This implementation is
// significantly faster than straight Ford-Fulkerson.  It solves
// random problems with 10000 vertices and 1000000 edges in a few
// seconds, though it is possible to construct test cases that
// achieve the worst-case.
//
// Running time:
//     O(|V|^3)
//
// INPUT: 
//     - graph, constructed using AddEdge()
//     - source
//     - sink
//
// OUTPUT:
//     - maximum flow value
//     - To obtain the actual flow values, look at all edges with
//       capacity > 0 (zero capacity edges are residual edges).

#include <cmath>
#include <vector>
#include <iostream>
#include <queue>

using namespace std;

typedef long long LL;

struct Edge {
  int from, to, cap, flow, index;
  Edge(int from, int to, int cap, int flow, int index) :
    from(from), to(to), cap(cap), flow(flow), index(index) {}
};

struct PushRelabel {
  int N;
  vector<vector<Edge> > G;
  vector<LL> excess;
  vector<int> dist, active, count;
  queue<int> Q;

  PushRelabel(int N) : N(N), G(N), excess(N), dist(N), active(N), count(2*N) {}

  void AddEdge(int from, int to, int cap) {
    G[from].push_back(Edge(from, to, cap, 0, G[to].size()));
    if (from == to) G[from].back().index++;
    G[to].push_back(Edge(to, from, 0, 0, G[from].size() - 1));
  }

  void Enqueue(int v) { 
    if (!active[v] && excess[v] > 0) { active[v] = true; Q.push(v); } 
  }

  void Push(Edge &e) {
    int amt = int(min(excess[e.from], LL(e.cap - e.flow)));
    if (dist[e.from] <= dist[e.to] || amt == 0) return;
    e.flow += amt;
    G[e.to][e.index].flow -= amt;
    excess[e.to] += amt;    
    excess[e.from] -= amt;
    Enqueue(e.to);
  }
  
  void Gap(int k) {
    for (int v = 0; v < N; v++) {
      if (dist[v] < k) continue;
      count[dist[v]]--;
      dist[v] = max(dist[v], N+1);
      count[dist[v]]++;
      Enqueue(v);
    }
  }

  void Relabel(int v) {
    count[dist[v]]--;
    dist[v] = 2*N;
    for (int i = 0; i < G[v].size(); i++) 
      if (G[v][i].cap - G[v][i].flow > 0)
  dist[v] = min(dist[v], dist[G[v][i].to] + 1);
    count[dist[v]]++;
    Enqueue(v);
  }

  void Discharge(int v) {
    for (int i = 0; excess[v] > 0 && i < G[v].size(); i++) Push(G[v][i]);
    if (excess[v] > 0) {
      if (count[dist[v]] == 1) 
  Gap(dist[v]); 
      else
  Relabel(v);
    }
  }

  LL GetMaxFlow(int s, int t) {
    count[0] = N-1;
    count[N] = 1;
    dist[s] = N;
    active[s] = active[t] = true;
    for (int i = 0; i < G[s].size(); i++) {
      excess[s] += G[s][i].cap;
      Push(G[s][i]);
    }
    
    while (!Q.empty()) {
      int v = Q.front();
      Q.pop();
      active[v] = false;
      Discharge(v);
    }
    
    LL totflow = 0;
    for (int i = 0; i < G[s].size(); i++) totflow += G[s][i].flow;
    return totflow;
  }
};

const int MAXW = 110;
const int MAXH = 510;

bool building[MAXW][MAXH];

const int dx[] = {0, 1, 0, -1};
const int dy[] = {1, 0, -1, 0};

int main() {
  int T;
  cin >> T;
  for (int re = 1; re <= T; ++re) {
    int w, h, n;
    cin >> w >> h >> n;
    memset(building, 0, sizeof(building));
    for (int i = 0; i < n; ++i) {
      int x, y, xx, yy;
      cin >> x >> y >> xx >> yy;
      for (int j = x; j <= xx; ++j) {
        for (int k = y; k <= yy; ++k) {
          building[j][k] = true;
        }
      }
    }

    PushRelabel d(w * h * 2 + 2);
    int s = w * h * 2;
    int t = w * h * 2 + 1;
    for (int i = 0; i < w; ++i) {
      for (int j = 0; j < h; ++j) {
        if (!building[i][j]) {
          //self -> self
          d.AddEdge(w * j + i, w * j + i + w * h, 1);
          for (int k = 0; k < 4; ++k) {
            int x = i + dx[k];
            int y = j + dy[k];
            if (x >= 0 && x < w && y >= 0 && y < h && !building[x][y]) {
              // self -> other
              d.AddEdge(w * j + i + w * h, w * y + x, 1);
            }
          }
        }
      }
    }

    for (int i = 0; i < w; ++i) {
      // in
      d.AddEdge(s, i, 1);
      // out
      d.AddEdge(w * (h - 1) + i + w * h, t, 1);
    }
    cout << "Case #" << re << ": " << d.GetMaxFlow(s, t) << endl;
  }
  return 0;
}