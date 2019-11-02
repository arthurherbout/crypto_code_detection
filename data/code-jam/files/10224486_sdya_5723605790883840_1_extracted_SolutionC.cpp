#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <bitset>
#include <cmath>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const long long INF = 1000000000000000LL;

class MaximumFlow {
	int s, f;
public:
	struct edge {
		int u, v;
		long long capacity, flow;
		edge() { u = v = capacity = flow = 0;}
		edge(int u_, int v_, long long capacity_, long long flow_) :
			u(u_), v(v_), capacity(capacity_), flow(flow_) {}
	};

	vector <edge> edges;
	vector <vector <int> > graph;
	vector <int> ptr, level;
	queue <int> q;
	int n;

	MaximumFlow() {}

	MaximumFlow(int number) {
		n = number;
		graph.resize(n);
		ptr.assign(n, 0);
		level.resize(n);
	}

	void addEdge(int u, int v, long long capacity) {
		int sz = (int)(edges.size());
		edges.push_back(edge(u, v, capacity, 0));
		edges.push_back(edge(v, u, 0, 0));
		graph[u].push_back(sz);
		graph[v].push_back(sz + 1);
	}

	void updateLevels() {
		level.assign(n, -1);
		q.push(s);
		level[s] = 0;
		while (!q.empty()) {
			int topq = q.front();
			q.pop();
			for (int index = 0; index < graph[topq].size(); ++index) {
				int i = graph[topq][index];
				int to = edges[i].v;
				if (edges[i].capacity - edges[i].flow == 0) {
					continue;
				}
				if (level[to] == -1) {
					level[to] = level[topq] + 1;
					q.push(to);
				}
			}
		}
	}

	long long pushFlow(int v, long long flow) {
		if (v == f || flow == 0) {
			return flow;
		}
		for (; ptr[v] < graph[v].size(); ++ptr[v]) {
			int index = graph[v][ptr[v]];
			int to = edges[index].v;
			if (level[v] + 1 == level[to]) {
				int pushed = pushFlow(to, min(flow, edges[index].capacity - edges[index].flow));
				if (pushed > 0) {
					edges[index].flow += pushed;
					edges[index ^ 1].flow -= pushed;
					return pushed;
				}
			}
		}
		return 0;
	}

	long long dinicFlow(int start, int finish) {
		s = start, f = finish;
		long long result = 0;
		while (true) {
			updateLevels();
			if (level[f] == -1) {
				break;
			}
			while (true) {
				long long pushed = pushFlow(start, INF);
				if (pushed == 0) {
					break;
				}
				result += pushed;
			}
			ptr.assign(n, 0);
		}
		return result;
	}
};

void readData() {
}

const int maxN = 2000;
int n, m;
int p[maxN];
pair < int, int > c[maxN];
int position[maxN];
char maze[maxN][maxN];

bool used[maxN][maxN][4];

void set0() {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			for (int k = 0; k < 4; ++k) {
				used[i][j][k] = false;
			}
		}
	}
}

void dfs2(int x, int y, int p) {
	if (x < 0 || x >= n || y < 0 || y >= m) {
		return;
	}
	if (used[x][y][p]) {
		return;
	}
	used[x][y][p] = true;
	if (p == 0) {
		dfs2(x - 1, y, 2);
		if (maze[x][y] == '/' || maze[x][y] == 0) {
			dfs2(x, y, 3);
		}
		if (maze[x][y] == '\\' || maze[x][y] == 0) {
			dfs2(x, y, 1);
		}
	}
	if (p == 1) {
		dfs2(x, y + 1, 3);
		if (maze[x][y] == '/' || maze[x][y] == 0) {
			dfs2(x, y, 2);
		}
		if (maze[x][y] == '\\' || maze[x][y] == 0) {
			dfs2(x, y, 0);
		}
	}
	if (p == 2) {
		dfs2(x + 1, y, 0);
		if (maze[x][y] == '/' || maze[x][y] == 0) {
			dfs2(x, y, 1);
		}
		if (maze[x][y] == '\\' || maze[x][y] == 0) {
			dfs2(x, y, 3);
		}
	}
	if (p == 3) {
		dfs2(x, y - 1, 1);
		if (maze[x][y] == '/' || maze[x][y] == 0) {
			dfs2(x, y, 0);
		}
		if (maze[x][y] == '\\' || maze[x][y] == 0) {
			dfs2(x, y, 2);
		}
	}
}

void dfs(int x, int y, int p) {
	if (x < 0 || x >= n || y < 0 || y >= m) {
		return;
	}
	if (used[x][y][p]) {
		return;
	}
	used[x][y][p] = true;
	if (p == 0) {
		dfs(x - 1, y, 2);
		if (maze[x][y] == '/') {
			dfs(x, y, 3);
		} else {
			dfs(x, y, 1);
		}
	}
	if (p == 1) {
		dfs(x, y + 1, 3);
		if (maze[x][y] == '/') {
			dfs(x, y, 2);
		} else {
			dfs(x, y, 0);
		}
	}
	if (p == 2) {
		dfs(x + 1, y, 0);
		if (maze[x][y] == '/') {
			dfs(x, y, 1);
		} else {
			dfs(x, y, 3);
		}
	}
	if (p == 3) {
		dfs(x, y - 1, 1);
		if (maze[x][y] == '/') {
			dfs(x, y, 0);
		} else {
			dfs(x, y, 2);
		}
	}
}

bool check2() {
	for (int i = 0; i < 2 * (n + m); i += 2) {
		int x = p[i];
		int y = p[i + 1];
		set0();
		dfs2(c[x].first, c[x].second, position[x]);

		if (!used[c[y].first][c[y].second][position[y]]) {
			return false;
		}
	}
	return true;
}

bool check() {
	for (int i = 0; i < 2 * (n + m); i += 2) {
		int x = p[i];
		int y = p[i + 1];
		set0();
		dfs(c[x].first, c[x].second, position[x]);

		if (!used[c[y].first][c[y].second][position[y]]) {
			return false;
		}

		for (int j = 0; j < 2 * (n + m); ++j) {
			if (j == i || j == i + 1) {
				continue;
			}
			int t = p[j];
			if (used[c[t].first][c[t].second][position[t]]) {
				return false;
			}
		}
	}
	return true;
}

int getId(int x, int y, int p, int source) {
	return (x * m * 4 + y * 4 + p) * 2 + source;
}

void solve2(int test) {
	cin >> n >> m;
	for (int i = 0; i < 2 * (n + m); ++i) {
		cin >> p[i];
		--p[i];
	}

	for (int i = 0; i < m; ++i) {
		c[i] = make_pair(0, i);
		position[i] = 0;
	}
	for (int i = m; i < m + n; ++i) {
		c[i] = make_pair(i - m, m - 1);
		position[i] = 1;
	}
	for (int i = m + n; i < m + m + n; ++i) {
		int y = i - m - n;
		y = m - 1 - y;
		c[i] = make_pair(n - 1, y);
		position[i] = 2;
	}
	for (int i = m + m + n; i < m + m + n + n; ++i) {
		int x = i - m - m - n;
		x = n - 1 - x;
		c[i] = make_pair(x, 0);
		position[i] = 3;
	}

	int cnt = n * m * 4 * 2 + 2;
	int s = cnt - 2;
	int t = cnt - 1;

	MaximumFlow F(cnt);

	for (int x = 0; x < n; ++x) {
		for (int y = 0; y < m; ++y) {
			for (int p = 0; p < 4; ++p) {
				F.addEdge(getId(x, y, p, 0), getId(x, y, p, 1), 1);

				int u = getId(x, y, p, 1);
				if (p == 0) {
					if (x >= 1) F.addEdge(u, getId(x - 1, y, 2, 0), 1);
					F.addEdge(u, getId(x, y, 3, 0), 1);
					F.addEdge(u, getId(x, y, 1, 0), 1);
				}
				if (p == 1) {
					if (y + 1 < m) F.addEdge(u, getId(x, y + 1, 3, 0), 1);
					F.addEdge(u, getId(x, y, 2, 0), 1);
					F.addEdge(u, getId(x, y, 0, 0), 1);
				}
				if (p == 2) {
					if (x + 1 < n) F.addEdge(u, getId(x + 1, y, 0, 0), 1);
					F.addEdge(u, getId(x, y, 1, 0), 1);
					F.addEdge(u, getId(x, y, 3, 0), 1);
				}
				if (p == 3) {
					if (y >= 1) F.addEdge(u, getId(x, y - 1, 1, 0), 1);
					F.addEdge(u, getId(x, y, 0, 0), 1);
					F.addEdge(u, getId(x, y, 2, 0), 1);
				}
			}
		}
	}

	for (int i = 0; i < 2 * (n + m); i += 2) {
		int x = p[i];
		int y = p[i + 1];

		F.addEdge(s, getId(c[x].first, c[x].second, position[x], 0), 1);
		F.addEdge(getId(c[y].first, c[y].second, position[y], 1), t, 1);
	}

	printf("Case #%d:\n", test);
	int flow = F.dinicFlow(s, t);
	if (flow != (n + m)) {
		cout << "IMPOSSIBLE" << endl;
	}
}

bool makeAttempt() {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			maze[i][j] = 0;
		}
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			char c1 = rand() % 2 ? '/' : '\\';
			char c2 = c1 == '/' ? '\\' : '/';
			maze[i][j] = c1;
			if (check2()) {
				continue;
			}
			maze[i][j] = c2;
			if (check2()) {
				continue;
			}
			return false;
		}
	}
	if (!check()) {
		return false;
	}
	return true;
}

void solve3(int test) {
	cin >> n >> m;
	for (int i = 0; i < 2 * (n + m); ++i) {
		cin >> p[i];
		--p[i];
	}
	for (int i = 0; i < m; ++i) {
		c[i] = make_pair(0, i);
		position[i] = 0;
	}
	for (int i = m; i < m + n; ++i) {
		c[i] = make_pair(i - m, m - 1);
		position[i] = 1;
	}
	for (int i = m + n; i < m + m + n; ++i) {
		int y = i - m - n;
		y = m - 1 - y;
		c[i] = make_pair(n - 1, y);
		position[i] = 2;
	}
	for (int i = m + m + n; i < m + m + n + n; ++i) {
		int x = i - m - m - n;
		x = n - 1 - x;
		c[i] = make_pair(x, 0);
		position[i] = 3;
	}

	printf("Case #%d:\n", test);

	bool good = false;
	for (int t = 0; t < 100; ++t) {
		if (makeAttempt()) {
			good = true;
			break;
		}
	}

	if (!good) {
		cout << "IMPOSSIBLE" << endl;
		return;
	}

	for (int x = 0; x < n; ++x) {
		for (int y = 0; y < m; ++y) {
			cout << maze[x][y];
		}
		cout << endl;
	}

	if (!check()) {
		cerr << "BAD!!!" << endl;
	}
}

void solve(int test) {
	cin >> n >> m;
	for (int i = 0; i < 2 * (n + m); ++i) {
		cin >> p[i];
		--p[i];
	}

	for (int i = 0; i < m; ++i) {
		c[i] = make_pair(0, i);
		position[i] = 0;
	}
	for (int i = m; i < m + n; ++i) {
		c[i] = make_pair(i - m, m - 1);
		position[i] = 1;
	}
	for (int i = m + n; i < m + m + n; ++i) {
		int y = i - m - n;
		y = m - 1 - y;
		c[i] = make_pair(n - 1, y);
		position[i] = 2;
	}
	for (int i = m + m + n; i < m + m + n + n; ++i) {
		int x = i - m - m - n;
		x = n - 1 - x;
		c[i] = make_pair(x, 0);
		position[i] = 3;
	}

	int t = n * m;
	printf("Case #%d:\n", test);
	for (int i = 0; i < (1 << t); ++i) {
		for (int x = 0; x < n; ++x) {
			for (int y = 0; y < m; ++y) {
				int j = x * m + y;
				if (i & (1 << j)) {
					maze[x][y] = '/';
				} else {
					maze[x][y] = '\\';
				}
			}
		}

		if (check()) {
			for (int x = 0; x < n; ++x) {
				for (int y = 0; y < m; ++y) {
					cout << maze[x][y];
				}
				cout << endl;
			}
			return;
		}
	}
	cout << "IMPOSSIBLE" << endl;
}

int main(int argc, char* argv[]) {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int left_bound, right_bound;

	/*freopen(argv[3], "w", stdout);	
	sscanf(argv[1], "%d", &left_bound);
	sscanf(argv[2], "%d", &right_bound);*/

	int t;
	cin >> t;
	left_bound = 1, right_bound = t;
	for (int i = 1; i <= t; ++i) {
		if (i >= left_bound && i <= right_bound) {
			solve3(i);
		}
		cerr << i << ": " << clock() << endl;
	}

	return 0;
}
