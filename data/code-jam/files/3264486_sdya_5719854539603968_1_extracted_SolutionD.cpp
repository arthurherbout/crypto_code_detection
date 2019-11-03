#define _CRT_SECURE_NO_WARNINGS
#pragma comment (linker, "/STACK:256000000")
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <set>
#include <map>
#include <bitset>
#include <ctime>
#include <queue>

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

pair < int, vector < pair < char, pair < int, int > > > > solveTrivial(vector < vector < char > > a, int n) {
	vector < string > r(n, string(n, '.'));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			r[i][j] = a[i][j];
		}
	}

	int index = -1;
	for (int i = 0; i < n; ++i) {
		if (r[0][i] == 0) {
			r[0][i] = '+';
		} else if (r[0][i] == 'x') {
			r[0][i] = 'o';
		}

		if (r[0][i] != '+') {
			index = i;
		}
	}
	if (index == -1) {
		r[0][0] = 'o';
		index = 0;
	}

	for (int i = 1; i + 1 < n; ++i) {
		r[n - 1][i] = '+';
	}

	vector < bool > x(n, false), y(n, false);
	y[index] = true;

	int cnt = 0;
	for (int i = n - 1; i >= 0; --i) {
		for (int j = 0; j < n; ++j) {
			if (r[i][j] == 0 && !x[i] && !y[j]) {
				r[i][j] = 'x';
				x[i] = y[j] = true;
				++cnt;
			}
		}
	}

	if (cnt != n - 1) {
		cerr << "BAD" << endl;
	}

	int score = 0;
	vector < pair < char, pair < int, int > > > res;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (r[i][j] == 'o') {
				score += 2;
				if (a[i][j] != 'o') {
					res.push_back(make_pair('o', make_pair(i + 1, j + 1)));
				}
			} else if (r[i][j] == 'x') {
				score += 1;
				if (a[i][j] != 'x') {
					res.push_back(make_pair('x', make_pair(i + 1, j + 1)));
				}
			} else if (r[i][j] == '+') {
				score += 1;
				if (a[i][j] != '+') {
					res.push_back(make_pair('+', make_pair(i + 1, j + 1)));
				}
			}
		}
	}

	return make_pair(score, res);
}

pair < int, vector < pair < char, pair < int, int > > > > solveCool(vector < vector < char > > a, int n, int tt = 0) {
	MaximumFlow F(2 * (2 * n - 1) + 2);
	int s = 2 * (2 * n - 1);
	int t = s + 1;

	vector < vector < bool > > candidate(n, vector < bool > (n, true));

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (a[i][j] == '+' || a[i][j] == 'o') {

				for (int u = 0; u < n; ++u) {
					for (int v = 0; v < n; ++v) {
						if (u - v == i - j || u + v == i + j) {
							candidate[u][v] = false;
						}
					}
				}

			}
		}
	}


	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (a[i][j] == 0 && candidate[i][j]) {
				F.addEdge(i + j, 2 * n - 1 + (i - j) + n - 1, 1);
			}
		}
	}

	for (int i = 0; i + 1 < 2 * n; ++i) {
		F.addEdge(s, i, 1);
		F.addEdge(i + 2 * n - 1, t, 1);
	}

	for (int i = 0; i < F.graph.size(); ++i) {
		if (tt != 0) {
			random_shuffle(F.graph[i].begin(), F.graph[i].end());
		}
	}

	int f1 = F.dinicFlow(s, t);

	vector < pair < int, int > > pluses;
	for (int i = 0; i < 2 * n - 1; ++i) {
		for (int j = 0; j < F.graph[i].size(); ++j) {
			int id = F.graph[i][j];

			if (F.edges[id].v >= 2 * n - 1 && F.edges[id].v < 2 * (2 * n - 1) && F.edges[id].flow > 0) {
				int a = i;
				int b = F.edges[id].v - (2 * n - 1) - (n - 1);

				int x = (a + b) / 2;
				int y = a - x;
				pluses.push_back(make_pair(x, y));
			}
		}
	}

	vector < string > r(n, string(n, 0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			r[i][j] = a[i][j];
		}
	}

	for (int i = 0; i < pluses.size(); ++i) {
		r[pluses[i].first][pluses[i].second] = '+';
	}

	candidate = vector < vector < bool > > (n, vector < bool > (n, true));

	int crosses = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (r[i][j] == 'x' || r[i][j] == 'o') {
				++crosses;
				for (int u = 0; u < n; ++u) {
					for (int v = 0; v < n; ++v) {
						if (u == i || v == j) {
							candidate[u][v] = false;
						}
					}
				}
			}
		}
	}

	MaximumFlow G(2 * n + 2);
	s = 2 * n;
	t = 2 * n + 1;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (r[i][j] == 0 && candidate[i][j]) {
				G.addEdge(i, j + n, 1);
			}
		}
	}
	for (int i = 0; i < n; ++i) {
		G.addEdge(s, i, 1);
		G.addEdge(i + n, t, 1);
	}

	int f2 = G.dinicFlow(s, t);
	if (f2 + crosses != n && f2 + crosses != n - 1) {
		cerr << "OOPS " << f1 << " " << f2 << " " << " " << crosses << " " << n - 1 << endl;
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < G.graph[i].size(); ++j) {
			int id = G.graph[i][j];
			if (G.edges[id].v >= n && G.edges[id].v < 2 * n && G.edges[id].flow > 0) {
				r[i][G.edges[id].v - n] = 'x';
			}
		}
	}

	bool marked = false;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (r[i][j] == '+' && !marked) {
				bool good = true;
				for (int u = 0; u < n; ++u) {
					for (int v = 0; v < n; ++v) {
						if (u == i || v == j) {
							if (r[u][v] == 'o' || r[u][v] == 'x') {
								good = false;
							}
						}
					}
				}
				if (good) {
					r[i][j] = 'o';
					marked = true;
				}
			}
			if (r[i][j] == 'x' && !marked) {
				bool good = true;
				for (int u = 0; u < n; ++u) {
					for (int v = 0; v < n; ++v) {
						if (u + v == i + j || u - v == i - j) {
							if (r[u][v] == 'o' || r[u][v] == '+') {
								good = false;
							}
						}
					}
				}
				if (good) {
					r[i][j] = 'o';
					marked = true;
				}
			}
		}
	}

	int score = 0;
	vector < pair < char, pair < int, int > > > res;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (r[i][j] == 'o') {
				score += 2;
				if (a[i][j] != 'o') {
					res.push_back(make_pair('o', make_pair(i + 1, j + 1)));
				}
			} else if (r[i][j] == 'x') {
				score += 1;
				if (a[i][j] != 'x') {
					res.push_back(make_pair('x', make_pair(i + 1, j + 1)));
				}
			} else if (r[i][j] == '+') {
				score += 1;
				if (a[i][j] != '+') {
					res.push_back(make_pair('+', make_pair(i + 1, j + 1)));
				}
			}
		}
	}
	return make_pair(score, res);
}

void solve(int test) {

	if (test == 57) {
		int f = 0;
	}
	int n, m;
	cin >> n >> m;

	vector < vector < char > > a(n, vector < char > (n, 0));
	vector < vector < int > > c(n, vector < int > (n, 7));

	for (int i = 0; i < m; ++i) {
		string s;
		int x, y;
		cin >> s >> x >> y;
		--x;
		--y;
		a[x][y] = s[0];
	}

	int score = 0;
	vector < pair < char, pair < int, int > > > res;
	for (int i = 0; i < 50; ++i) {
		pair < int, vector < pair < char, pair < int, int > > > > u =solveCool(a, n, i);
		if (u.first > score) {
			score = u.first;
			res = u.second;
			cerr << "update" << endl;
		}
	}

	printf("Case #%d: %d %d\n", test, score, res.size());
	for (int i = 0; i < res.size(); ++i) {
		printf("%c %d %d\n", res[i].first, res[i].second.first, res[i].second.second);
	}
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int t;
	scanf("%d", &t);
	for (int i = 0; i < t; ++i) {
		solve(i + 1);
		cerr << i + 1 << ": " << clock() << endl;
	}

	return 0;
}