#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
#define VAR(x) ((x) << 1)
#define NOT(x) ((x) ^ 1)
typedef int Weight;
struct edge {
  int src, dst;
  edge(int src, int dst) :
    src(src), dst(dst) { }
};
bool operator < (const edge &e, const edge &f) {
  return 
    e.src != f.src ? e.src < f.src : e.dst < f.dst;
}
typedef vector<edge> edges;
typedef vector<edges> graph;

void visit(int v, const graph &g,
    vector<int> &ord, vector<int> &num, int k) {
  if (num[v] >= 0) return;
  num[v] = k;
  for (int i = 0; i < g[v].size(); ++i)
    visit(g[v][i].dst, g, ord, num, k);
  ord.push_back(v);
}
typedef pair<int,int> clause;
pair<bool, vector<int> > two_satisfiability(int m, const vector<clause> &cs) {
  int n = m * 2; // m positive vars and m negative vars
  graph g(n), h(n);
  for (int i = 0; i < cs.size(); ++i) {
    int u = cs[i].first, v = cs[i].second;
    g[NOT(u)].push_back( edge(NOT(u), v) );
    g[NOT(v)].push_back( edge(NOT(v), u) );
    h[v].push_back( edge(v, NOT(u)) );
    h[u].push_back( edge(u, NOT(v)) );
  }
  vector<int> num(n, -1), ord, dro;
  for (int i = 0; i < n; ++i)
    visit(i, g, ord, num, i);
  reverse(ord.begin(), ord.end());
  fill(num.begin(), num.end(), -1);
  for (int i = 0; i < n; ++i)
    visit(ord[i], h, dro, num, i);
  for (int i = 0; i < n; ++i)
    if (num[i] == num[NOT(i)])
      return make_pair(false, vector<int>());

	vector<int> ret(m);
	for (int i = 0; i < n; i+= 2) {
		ret[i/2] = num[i] > num[NOT(i)] ? 1 : 0;
	}
	return make_pair(true, ret);
}


bool test(vector<string>& m, int x, int y, int dx, int dy) {
	if (x < 0 || x >= m.size() || y < 0 || y >= m[0].length()) {
		return true;
	}
	if (m[x][y] == '|' || m[x][y] == '-') {
		return false;
	}
	if (m[x][y] == '#') {
		return true;
	}
	return test(m, x + dx, y + dy, dx, dy);
}

void shoot(vector<string>& m, int x, int y, int dx, int dy) {
	if (x < 0 || x >= m.size() || y < 0 || y >= m[0].length()) {
		return;
	}
	if (m[x][y] == '#') {
		return;
	}
	if (m[x][y] == '.') {
		m[x][y] = ' ';
	}
	shoot(m, x + dx, y + dy, dx, dy);
}

int find(vector<string>& m, vector<vector<int> >& revT, int x, int y, int dx, int dy) {
	if (x < 0 || x >= m.size() || y < 0 || y >= m[0].length()) {
		return -1;
	}
	if (m[x][y] == '#') {
		return -1;
	}
	if (revT[x][y] != -1) {
		return revT[x][y];
	}
	return find(m, revT, x+dx, y+dy, dx, dy);
}

int main() {
	int T; cin >> T;
	for (int No = 1; No <= T; No++) {
		int R, C; cin >> R >> C;
		vector<string> M(R);
		for (int i = 0; i < R; i++) cin >> M[i];

		bool impossible = false;
		vector<pair<int, int> > target;
		vector<vector<int> > revTarget(R, vector<int>(C, -1));
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				if (M[i][j] == '|' || M[i][j] == '-') {
					bool v = test(M, i+1, j, 1, 0) && test(M, i-1, j, -1, 0);
					bool h = test(M, i, j+1, 0, 1) && test(M, i, j-1, 0, -1);
					if (v && h) {
						revTarget[i][j] = target.size();
						target.push_back(make_pair(i, j));
					} else if (v) {
						M[i][j] = '|';
						shoot(M, i+1, j, 1, 0);
						shoot(M, i-1, j, -1, 0);
					} else if (h) {
						M[i][j] = '-';
						shoot(M, i, j+1, 0, 1);
						shoot(M, i, j-1, 0, -1);
					} else {
						impossible = true;
					}
				}
			}
		}
		vector<clause> cs;
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				if (M[i][j] == '.') {
					int a = find(M, revTarget, i+1, j, 1, 0);
					int b = find(M, revTarget, i-1, j, -1, 0);
					int c = find(M, revTarget, i, j+1, 0, 1);
					int d = find(M, revTarget, i, j-1, 0, -1);
					vector<int> s;
					if (a != -1) s.push_back(VAR(a));
					if (b != -1) s.push_back(VAR(b));
					if (c != -1) s.push_back(NOT(VAR(c)));
					if (d != -1) s.push_back(NOT(VAR(d)));
					if (s.size() == 0) {
						impossible = true;
					} else {
						cs.push_back(make_pair(s[0], s.size() > 1 ? s[1] : s[0]));
					}
				}
			}
		}
		pair<bool, vector<int> > ret = two_satisfiability(target.size(), cs);	
		if (ret.first == false) impossible = true;

		if (impossible) {
			cout << "Case #" << No << ": IMPOSSIBLE" << endl;
		} else {
			cout << "Case #" << No << ": POSSIBLE" << endl;
			for (int i = 0; i < ret.second.size(); i++) {
				pair<int, int> p = target[i];
				M[p.first][p.second] = ret.second[i] ? '|' : '-';
			}
			for (int i = 0; i < R; i++) for (int j = 0; j < C; j++) if (M[i][j] == ' ') M[i][j] == '.';
			for (int i = 0; i < R; i++) cout << M[i] << endl;
		}
	}
}
