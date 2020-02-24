//C = 2のケース
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#define RESIDUE(s,t) (capacity[s][t]-flow[s][t])
#define REP(i,n) for(int i=0;i<(int)n;++i)
#define FOR(i,c) for(__typeof((c).begin())i=(c).begin();i!=(c).end();++i)
#define ALL(c) (c).begin(), (c).end()
using namespace std;

//ライブラリ
const int INF = 11451419;
typedef int Weight;
struct Edge {
  int src, dst;
  Weight weight;
  Edge(int src, int dst, Weight weight) :
    src(src), dst(dst), weight(weight) { }
};
bool operator < (const Edge &e, const Edge &f) {
  return e.weight != f.weight ? e.weight > f.weight : // !!INVERSE!!
    e.src != f.src ? e.src < f.src : e.dst < f.dst;
}
typedef vector<Edge> Edges;
typedef vector<Edges> Graph;

typedef vector<Weight> Array;
typedef vector<Array> Matrix;

//ライブラリ2
Weight augment(const Graph &g, const Matrix &capacity, Matrix &flow,
    const vector<int> &level, vector<bool> &finished, int u, int t, Weight cur) {
  if (u == t || cur == 0) return cur;
  if (finished[u]) return 0;
  finished[u] = true;
  for (int i = 0; i < g[u].size(); i++) {
  	Edge e = g[u][i];
  	if (level[e.dst] > level[u]) {
	    Weight f = augment(g, capacity, flow, level, finished, e.dst, t, min(cur, RESIDUE(u, e.dst)));
	    if (f > 0) {
	      flow[u][e.dst] += f; flow[e.dst][u] -= f;
	      finished[u] = false;
	      return f;
    	}
  	}
  }
  return 0;
}
Weight maximumFlow(const Graph &g, int s, int t) {
  int n = g.size();
  Matrix flow(n, Array(n)), capacity(n, Array(n)); // adj. matrix
  REP(u,n) {
  	for (int i = 0; i < g[u].size(); i++) { Edge e = g[u][i]; capacity[e.src][e.dst] += e.weight; }
  }

  Weight total = 0;
  for (bool cont = true; cont; ) {
    cont = false;
    vector<int> level(n, -1); level[s] = 0; // make layered network
    queue<int> Q; Q.push(s);
    for (int d = n; !Q.empty() && level[Q.front()] < d; ) {
      int u = Q.front(); Q.pop();
      if (u == t) d = level[u];
      for (int i = 0; i < g[u].size(); i++) {
      	 Edge e = g[u][i];
      	 if (RESIDUE(u,e.dst) > 0 && level[e.dst] == -1)
         Q.push(e.dst), level[e.dst] = level[u] + 1;
      }
    }
    vector<bool> finished(n); // make blocking flows
    for (Weight f = 1; f > 0; ) {
      f = augment(g, capacity, flow, level, finished, s, t, INF);
      if (f == 0) break;
      total += f; cont = true;
    }
  }
  return total;
}


//ここから
int t;
int n, c, m;
int p[1000], b[1000];
int cnt[2][1000];		//cnt[i][j] = 客iが席jを買った枚数

int solve1() {
	int A[2] = {0};
	int B[2] = {0};
	int i;
	
	A[0] = cnt[0][0];
	for (i = 1; i < n; i++) A[1] += cnt[0][i];
	B[0] = cnt[1][0];
	for (i = 1; i < n; i++) B[1] += cnt[1][i];
	
	int ret = 0;
	
	ret += min(A[0], B[1]);
	A[0] -= min(A[0], B[1]);
	B[1] -= min(A[0], B[1]);
	
	ret += min(B[0], A[1]);
	A[1] -= min(B[0], A[1]);
	B[0] -= min(B[0], A[1]);
	
	ret += min(A[1], B[1]);
	return ret;
}

//操作前の最大マッチング数
Graph g;
int solve2() {
	int s = 0;
	int t = 2 * n + 1;
	g.clear();
	g.resize(2 * n + 2);
	
	for (int i = 0; i < n; i++) {
		g[s].push_back(Edge(s, i + 1, cnt[0][i]));
		g[n + i + 1].push_back(Edge(n + i + 1, t, cnt[1][i]));
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == j) continue;
			if (cnt[0][i] == 0 || cnt[1][j] == 0) continue;
			g[i + 1].push_back(Edge(i + 1, n + j + 1, min(cnt[0][i], cnt[1][j])));
		}
	}
	
	return maximumFlow(g, s, t);
}

signed main() {
	cin >> t;
	
	for (int id = 1; id <= t; id++) {
		cin >> n >> c >> m;
		for (int i = 0; i < 2; i++) for (int j = 0; j < n; j++) cnt[i][j] = 0;
		for (int i = 0; i < m; i++) { cin >> p[i] >> b[i]; p[i]--; b[i]--; }
		for (int i = 0; i < m; i++) { cnt[b[i]][p[i]]++; }
		int res1 = solve1();
		int res2 = solve2();
		cout << "Case #" << id << ": " << m - res1 << " " << res1 - res2 << endl;
	}
	return 0;
}