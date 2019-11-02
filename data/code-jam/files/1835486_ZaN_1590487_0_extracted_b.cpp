#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

int n;
int s;
vector<vector<int> > b;
vector<vector<int> > c;

vector<int> cc;
vector<set<int> > ne;
vector<int> nc;

int check() {
	vector<pair<int, int> > q;
	vector<char> v(n*n);
	for (int i = 2; i < s; ++i) {
		if (!c[i][1]) {
			q.push_back(make_pair(i, 1));
			v[i*n + 1] = 1;
		}
		if (!c[1][i]) {
			q.push_back(make_pair(1, i));
			v[1*n + i] = 1;
		}
		if (!c[i][i + s - 1]) {
			q.push_back(make_pair(i, i + s - 1));
			v[i*n + i + s - 1] = 1;
		}
		if (!c[i + s - 1][i]) {
			q.push_back(make_pair(i + s - 1, i));
			v[(i + s - 1)*n + i] = 1;
		}
		if (!c[n - 1][i + s - 1]) {
			q.push_back(make_pair(n - 1, i + s - 1));
			v[(n - 1)*n + i + s - 1] = 1;
		}
		if (!c[i + s - 1][n - 1]) {
			q.push_back(make_pair(i + s - 1, n - 1));
			v[(i + s - 1)*n + (n - 1)] = 1;
		}
	}
	if (!c[1][1]) {
		q.push_back(make_pair(1, 1));
		v[1*n + 1] = 1;
	}
	if (!c[1][s]) {
		q.push_back(make_pair(1, s));
		v[1*n + s] = 1;
	}
	if (!c[s][1]) {
		q.push_back(make_pair(s, 1));
		v[s*n + 1] = 1;
	}
	if (!c[s][n - 1]) {
		q.push_back(make_pair(s, n - 1));
		v[s*n + n - 1] = 1;
	}
	if (!c[n - 1][s]) {
		q.push_back(make_pair(n - 1, s));
		v[(n - 1)*n + s] = 1;
	}
	if (!c[n - 1][n - 1]) {
		q.push_back(make_pair(n - 1, n - 1));
		v[(n - 1)*n + n - 1] = 1;
	}
	int pos = 0;
	while (pos < q.size()) {
		int ci = q[pos].first;
		int cj = q[pos].second;
		++pos;
		for (int di = -1; di <= 1; ++di) {
			for (int dj = -1; dj <= 1; ++dj) {
				int i = ci + di;
				int j = cj + dj;
				if (i < 1 || i >= n || j < 1 || j >= n || !b[i][j] || di*dj == -1) {
					continue;
				}
				if (!c[i][j] && !v[i*n + j]) {
					q.push_back(make_pair(i, j));
					v[i*n + j] = 1;
				}
			}
		}
	}
	return q.size();
}

void repaint(int from, int to) {
	for (int i = 1; i < cc.size(); ++i) {
		if (cc[i] == from) {
			cc[i] = to;
			ne[to].insert(ne[from].begin(), ne[from].end());
			nc[to] += nc[from];
		}
	}
}

int main() {
	int tests;
	cin >> tests;
	for (int test = 1; test <= tests; ++test) {
		int m;
		cin >> s >> m;
		n = s*2;
		b.assign(n, vector<int>(n, 1));
		c.assign(n, vector<int>(n));
		for (int i = 1; i < s; ++i) {
			for (int j = i + s; j < n; ++j) {
				b[i][j] = 0;
				b[j][i] = 0;
			}
		}
		for (int i = 2; i < s; ++i) {
			b[i][1] = 2;
			b[1][i] = 3;
			b[i][i + s - 1] = 4;
			b[i + s - 1][i] = 5;
			b[n - 1][i + s - 1] = 6;
			b[i + s - 1][n - 1] = 7;
		}
		b[1][1] = 8;
		b[1][s] = 8;
		b[s][1] = 8;
		b[s][n - 1] = 8;
		b[n - 1][s] = 8;
		b[n - 1][n - 1] = 8;

		cc.resize(1);
		ne.resize(1);
		nc.resize(1);
		bool has_ring = false;
		bool has_bridge = false;
		bool has_fork = false;
		int move;
		for (move = 1; move <= m; ++move) {
			int mi, mj;
			cin >> mi >> mj;
			bool check_ring = false;
			for (int di = -1; di <= 1; ++di) {
				for (int dj = -1; dj <= 1; ++dj) {
					for (int di2 = -1; di2 <= 1; ++di2) {
						for (int dj2 = -1; dj2 <= 1; ++dj2) {
							int i = mi + di;
							int j = mj + dj;
							int i2 = mi + di2;
							int j2 = mj + dj2;
							if (i < 1 || i >= n || j < 1 || j >= n || !b[i][j] || di*dj == -1 ||
									i2 < 1 || i2 >= n || j2 < 1 || j2 >= n || !b[i2][j2] || di2*dj2 == -1) {
								continue;
							}
							if (c[i][j] && cc[c[i2][j2]] == cc[c[i][j]] &&
									!((i == i2 && abs(j - j2) <= 1) ||
									  (j == j2 && abs(i - i2) <= 1) ||
									  (abs(i - i2) == 1 && i - i2 == j - j2))) {
								check_ring = true;
							}
						}
					}
				}
			}
			int color = 0;
			for (int di = -1; di <= 1; ++di) {
				for (int dj = -1; dj <= 1; ++dj) {
					int i = mi + di;
					int j = mj + dj;
					if (i < 1 || i >= n || j < 1 || j >= n || !b[i][j] || di*dj == -1) {
						continue;
					}
					if (c[i][j]) {
						if (!color) {
							color = cc[c[i][j]];
						} else if (cc[c[i][j]] != color) {
							repaint(cc[c[i][j]], color);
						}
					}
				}
			}
			if (!color) {
				color = cc.size();
				cc.push_back(color);
				ne.push_back(set<int>());
				nc.push_back(0);
			}
			c[mi][mj] = color;
			if (b[mi][mj] > 1 && b[mi][mj] < 8) {
				ne[color].insert(b[mi][mj]);
			} else if (b[mi][mj] == 8) {
				++nc[color];
			}

			has_bridge = (nc[color] >= 2);
			has_fork = (ne[color].size() >= 3);
			has_ring = false;
			if (check_ring) {
				int total = (n - 1)*(n - 1) - s*(s - 1);
				if (check() + move < total) {
					has_ring = true;
				}
			}

			if (has_bridge || has_fork || has_ring) {
				break;
			}
		}

		cout << "Case #" << test << ": ";
		if (has_bridge && has_fork && has_ring) {
			cout << "bridge-fork-ring in move " << move;
		} else if (has_fork && has_ring) {
			cout << "fork-ring in move " << move;
		} else if (has_bridge && has_fork) {
			cout << "bridge-fork in move " << move;
		} else if (has_bridge && has_ring) {
			cout << "bridge-ring in move " << move;
		} else if (has_fork) {
			cout << "fork in move " << move;
		} else if (has_ring) {
			cout << "ring in move " << move;
		} else if (has_bridge) {
			cout << "bridge in move " << move;
		} else {
			cout << "none";
		}
		cout << endl;
		++move;
		for (; move <= m; ++move) {
			int mi, mj;
			cin >> mi >> mj;
		}
	}
}
