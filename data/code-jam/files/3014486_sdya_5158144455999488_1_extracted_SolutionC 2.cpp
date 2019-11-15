#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <string>
#include <set>
#include <map>
#include <queue>
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

void read_data(int test) {
}

const int maxN = 1200;
int w, h, b;
int a[maxN][maxN];

bool valid(int x, int y) {
	return 0 <= x && x < w && 0 <= y && y < h;
}

int dx[] = {-1, 0, 0, 1};
int dy[] = { 0,-1, 1, 0};

vector < pair < int, int > > g[maxN];
const long long inf = 1000000000000000000LL;
long long d[maxN];

void solve(int test) {
	memset(a, 0, sizeof(a));
	cin >> w >> h >> b;
	vector < int > x0(b), y0(b), x1(b), y1(b);
	for (int i = 0; i < b; ++i) {
		cin >> x0[i] >> y0[i] >> x1[i] >> y1[i];
	}
	int n = b + 2;
	for (int i = 0; i < n; ++i) {
		g[i].clear();
		d[i] = inf;
	}
	for (int i = 0; i < b; ++i) {
		for (int j = i + 1; j < b; ++j) {
			int lx = max(x0[i], x0[j]);
			int rx = min(x1[i], x1[j]);

			int ly = max(y0[i], y0[j]);
			int ry = min(y1[i], y1[j]);

			/*if (lx <= rx) {
				int cap = max(y0[i], y0[j]) - min(y1[i], y1[j]) - 1;
				F.addEdge(i, j, cap);
				F.addEdge(j, i, cap);
			}
			if (ly <= ry) {
				int cap = max(x0[i], x0[j]) - min(x1[i], x1[j]) - 1;
				F.addEdge(i, j, cap);
				F.addEdge(j, i, cap);
			}

			if (lx > rx && ly > ry) {*/
				int cap = max(max(x0[i], x0[j]) - min(x1[i], x1[j]), max(y0[i], y0[j]) - min(y1[i], y1[j])) - 1;
				if (cap >= 0) {
					g[i].push_back(make_pair(j, cap));
					g[j].push_back(make_pair(i, cap));
				}
				//F.addEdge(i, j, cap);
				//F.addEdge(j, i, cap);
			//}
		}

		//F.addEdge(i, b, y0[i]);
		//F.addEdge(b, i, y0[i]);

		//F.addEdge(i, b + 1, x0[i]);
		//F.addEdge(b + 1, i, x0[i]);

		//F.addEdge(i, b + 2, w - 1 - x1[i]);
		//F.addEdge(b + 2, i, w - 1 - x1[i]);

		g[b].push_back(make_pair(i, x0[i]));
		g[i].push_back(make_pair(b, x0[i]));

		g[b + 1].push_back(make_pair(i, w - 1 - x1[i]));
		g[i].push_back(make_pair(b + 1, w - 1 - x1[i]));

		//F.addEdge(i, b + 3, h - 1 - y1[i]);
		//F.addEdge(b + 3, i, h - 1 - y1[i]);
	}

	int s = b;
	int t = b + 1;

	g[s].push_back(make_pair(t, w));
	g[t].push_back(make_pair(s, w));

	d[s] = 0;
	set < pair < long long, int > > S;
	S.insert(make_pair(d[s], s));
	while (!S.empty()) {
		int v = S.begin()->second;
		S.erase(S.begin());
		for (int i = 0; i < g[v].size(); ++i) {
			int w = g[v][i].first;
			long long len = g[v][i].second;

			if (d[w] > d[v] + len) {
				S.erase(make_pair(d[w], w));
				d[w] = d[v] + len;
				S.insert(make_pair(d[w], w));
			}
		}
	}

	printf("Case #%d: %d\n", test, (int)(d[t]));
	/*for (int t = 0; t < b; ++t) {
		int x0, y0, x1, y1;
		cin >> x0 >> y0 >> x1 >> y1;
		for (int i = x0; i <= x1; ++i) {
			for (int j = y0; j <= y1; ++j) {
				a[i][j] = 1;
			}
		}
	}

	int s = 2 * w * h;
	int t = s + 1;

	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < h; ++j) {
			if (a[i][j] == 1) {
				continue;
			}
			f.addEdge(i * h + j, w * h + i * h + j, 1);
			for (int k = 0; k < 4; ++k) {
				int nx = i + dx[k];
				int ny = j + dy[k];
				if (valid(nx, ny)) {
					f.addEdge(w * h + i * h + j, nx * h + ny, 1);
				}
			}
		}
	}

	for (int i = 0; i < w; ++i) {
		f.addEdge(s, i * h, 1);
		f.addEdge(w * h + i * h + (h - 1), t, 1);
	}
	printf("Case #%d: %d\n", test, (int)(f.dinicFlow(s, t)));*/
}

int main(int argc, char* argv[]) {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	//freopen("input.txt", "r", stdin);
	//freopen(argv[3], "w", stdout);

	int left_bound, right_bound;
	//sscanf(argv[1], "%d", &left_bound);
	//sscanf(argv[2], "%d", &right_bound);

	int tests;
	scanf("%d", &tests);
	left_bound = 1;
	right_bound = tests;
	for (int i = 1; i <= tests; ++i) {
		if (i >= left_bound && i <= right_bound) {
			solve(i);
			cerr << i << ": " << clock() << endl;
		} else {
			read_data(i);
		}
	}
	return 0;
}