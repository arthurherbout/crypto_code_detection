#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>
#include <vector>
#include <queue>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <numeric>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

int nextInt() {
	int x;
	scanf("%d", &x);
	return x;
}

double nextDouble() {
	double x;
	scanf("%lf", &x);
	return x;
}

const int BUF_SIZE = 2111111;
char buf[BUF_SIZE];

string nextString() {
	scanf("%s", buf);
	return buf;
}

struct Point {
	double x, y;
	Point () : x(0), y(0) {}
	Point (double x, double y) : x(x), y(y) {}
	Point operator - (Point op) { return Point(x - op.x, y - op.y); }
	Point operator + (Point op) { return Point(x + op.x, y + op.y); }
	Point operator * (double op) { return Point(x * op, y * op); }
	double operator % (Point op) { return x * op.y - y * op.x; }
	double operator * (Point op) { return x * op.x + y * op.y; }
};

template<class T>
T sqr(T x) {
	return x * x;
}

const double eps = 1e-9;

int cmp(double x, double y) {
	return x - y < -eps ? -1 : x - y > eps ? 1 : 0;
}

int gcd(int a, int b) {
	return b == 0 ? a : gcd(b, a % b);
}

///////////////////////////////////////////////////////////

typedef vector<vector<int> > Adj;

Adj buildGraph(int n, vector<int> &from, vector<int> &to) {
	Adj adjAfter(n);
	Adj adjBefore(n);
	for (int i = 0; i + 1 < n; ++i) {
		adjAfter[i].push_back(i + 1);
	}
	adjAfter[0].push_back(n - 1);
	for (int i = 1; i < n; ++i) {
		adjBefore[i].push_back(i - 1);
	}
	adjAfter[n - 1].push_back(0);
	for (int i = 0; i < from.size(); ++i) {
		if (from[i] < to[i]) {
			adjAfter[from[i]].push_back(to[i]);
			adjBefore[to[i]].push_back(from[i]);
		} else {
			adjAfter[to[i]].push_back(from[i]);
			adjBefore[from[i]].push_back(to[i]);
		}
	}
	for (int i = 0; i < adjBefore.size(); ++i) {
		sort(adjBefore[i].begin(), adjBefore[i].end());
	}
	for (int i = 0; i < adjAfter.size(); ++i) {
		sort(adjAfter[i].begin(), adjAfter[i].end());
	}
	Adj adj(n);
	for (int i = 0; i < n; ++i) {
		adj[i].insert(adj[i].end(), adjAfter[i].begin(), adjAfter[i].end());
		adj[i].insert(adj[i].end(), adjBefore[i].begin(), adjBefore[i].end());
	}
	return adj;
}

void dfs(int at, int to, Adj &adj, Adj &used, set<int> &c, vector<int> &colors) {
	used[at][to] = true;
	c.insert(colors[at]);
	c.insert(colors[to]);
	int n = adj.size();
	int curDist = (at - to + n) % n;
	for (int i = (int)adj[to].size() - 1; i >= 0; --i) {
		int nto = adj[to][i];
		int nDist = (nto - to + n) % n;
		if (nDist < curDist) {
			if (!used[to][nto]) {
				dfs(to, nto, adj, used, c, colors);
			}
			break;
		}
	}
}


bool validColors(vector<int> &c, vector<int> &from, vector<int> &to) {
	int colorsCount = *max_element(c.begin(), c.end()) + 1;
	Adj adj = buildGraph(c.size(), from, to);
	Adj used(c.size(), vector<int>(c.size()));
	for (int i = 0; i < adj.size(); ++i) {
		int to = (i + 1) % adj.size();
		if (!used[i][to]) {
			set<int> s;
			dfs(i, to, adj, used, s, c);
			if (s.size() != colorsCount) {
				return false;
			}
		}
	}
	return true;
}

int deb;

vector<int> bestColors;

bool go(int at, vector<int> &c, int cnt, vector<int> &from, vector<int> &to) {
	++deb;
	if (at == c.size()) {
		if (validColors(c, from, to)) {
			if (*max_element(c.begin(), c.end()) > *max_element(bestColors.begin(), bestColors.end())) {
				bestColors = c;
			}
		}
		return false;
	}
	for (int i = 0; i <= cnt; ++i) {
		c[at] = i;
		if (go(at + 1, c, cnt + (i == cnt), from, to)) {
			return false;
		}
		c[at] = -1;
	}
	return false;
}

int brute(int n, vector<int> &from, vector<int> &to, vector<int> &colors) {
	colors = vector<int>(n, -1);
	deb = 0;
	bestColors = colors;
	go(0, colors, 0, from, to);
	colors = bestColors;
	cerr << "deb = " << deb << endl;
	return *max_element(colors.begin(), colors.end()) + 1;
}

int main() {
	freopen("C-small-attempt0.in", "rt", stdin);
	freopen("C-small-attempt0.out", "wt", stdout);
	int T = nextInt();
	for (int cas = 1; cas <= T; ++cas) {
		int n = nextInt();
		int m = nextInt();
		vector<int> from(m), to(m);
		for (int i = 0; i < m; ++i) {
			from[i] = nextInt() - 1;
		}
		for (int i = 0; i < m; ++i) {
			to[i] = nextInt() - 1;
		}
		cerr << cas << endl;
		vector<int> colors;
		int res = brute(n, from, to, colors);
		cout << "Case #" << cas << ": " << res << endl;
		for (int i = 0; i < colors.size(); ++i) {
			if (i > 0) {
				cout << " ";
			}
			cout << colors[i] + 1;
		}
		cout << endl;
	}
	return 0;
}


