// Alex Fetisov

#include <cstdio>

void initf() {
	freopen("d.in", "r", stdin);
	freopen("d.out", "w", stdout);
}

#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <string>

using namespace std;

int n, m;
string f[12];
bool u[12][12];

vector < pair < int, int > > calc1(int x, int y) {
	queue < pair < int, int > > q;
	q.push(make_pair(x, y));
	memset(u, false, sizeof u);
	u[x][y] = true;
	vector < pair < int, int > > ret;
	while (!q.empty()) {
		pair < int, int > c = q.front();
		q.pop();
		ret.push_back(c);
		int cx = c.first;
		int cy = c.second;
		if (cx > 0) {
			int nx = cx - 1;
			int ny = cy;
			if (f[nx][ny] != '#') {
				if (!u[nx][ny]) {
					u[nx][ny] = true;
					q.push(make_pair(nx, ny));
				}
			}
		}
		if (cy > 0) {
			int nx = cx;
			int ny = cy - 1;
			if (f[nx][ny] != '#') {
				if (!u[nx][ny]) {
					u[nx][ny] = true;
					q.push(make_pair(nx, ny));
				}
			}
		}
		if (cy < m-1) {
			int nx = cx;
			int ny = cy + 1;
			if (f[nx][ny] != '#') {
				if (!u[nx][ny]) {
					u[nx][ny] = true;
					q.push(make_pair(nx, ny));
				}
			}
		}
	}
	return ret;
}

int needx, needy;

bool rec(int step, vector < pair < int, int > > v) {
	bool ok = true;
	for (int i = 0; i < v.size(); ++i) {
		if (v[i] != make_pair(needx, needy)) {
			ok = false;
			break;
		}
	}
	if (ok) return true;
	if (step > 14) {
		return false;
	}
	

	// try down
	ok = true;
	for (int i = 0; i < v.size(); ++i) {
		if (v[i].first + 1 > needx && v[i].first < n-1 && f[v[i].first + 1][v[i].second] != '#') {
			ok = false;
			break;
		}
	}
	vector < pair < int, int > > nv;
	if (ok) {
		for (int i = 0; i < v.size(); ++i) {
			if (f[v[i].first + 1][v[i].second] != '#') {
				nv.push_back(make_pair(v[i].first + 1, v[i].second));
			} else {
				nv.push_back(v[i]);
			}
		}
		if (rec(step + 1, nv)) return true;
	}
	//try left
	ok = true;
	nv.clear();
	if (ok) {
		for (int i = 0; i < v.size(); ++i) {
			if (v[i].second - 1 >= 0 && f[v[i].first][v[i].second-1] != '#') {
				nv.push_back(make_pair(v[i].first, v[i].second-1));
			} else {
				nv.push_back(v[i]);
			}
		}
		if (rec(step + 1, nv)) return true;
	}
	//try right
	ok = true;
	nv.clear();
	if (ok) {
		for (int i = 0; i < v.size(); ++i) {
			if (v[i].second + 1 < m && f[v[i].first][v[i].second+1] != '#') {
				nv.push_back(make_pair(v[i].first, v[i].second+1));
			} else {
				nv.push_back(v[i]);
			}
		}
		if (rec(step + 1, nv)) return true;
	}
	return false;
}

string calc2(int x, int y, vector < pair < int, int > > v) {
	needx = x;
	needy = y;
	if (rec(0, v)) {
		return "Lucky";
	} else {
		return "Unlucky";
	}
}

void solve() {
	cin >> n >> m;
	for (int i = 0; i < n; ++i) {
		cin >> f[i];
	}
	printf("\n");
	for (char c = '0'; c <= '9'; ++c) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				if (f[i][j] == c) {
					vector < pair < int, int > > p = calc1(i, j);
					printf("%c: %d ", c, p.size());	
					string ret = calc2(i, j, p);
					cout << ret << endl;
				}
			}
		}
	}
}

int main() {
	initf();
	int Test;
	scanf("%d", &Test);
	for (int i = 1; i <= Test; ++i) {
		printf("Case #%d: ", i);
		solve();
	}
	return 0;
}