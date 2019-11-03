#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <sstream>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
#define SZ(x) (int)(x.size())
#define F0(i,n) for(int i=0;i<n;i++)
#define F1(i,n) for(int i=1;i<=n;i++)
const int MOD = 1000002013;
const double pi = atan(1.0)*4.0;
const int inf = 1000000009;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

int i, j, k, m, n, l;
int d[105][105], nosol;
int X1[505], X2[505], Y1[505], Y2[505], connect[505], v[505][505], lastk[505][505];

void add(int x1, int y1, int x2, int y2) {
	X1[k] = x1; Y1[k] = y1; X2[k] = x2; Y2[k] = y2; k++;
}
int dx[] = { -1, 1, -1, 1 };
int dy[] = { 1, 1, -1, -1 };

int single(int x1, int y1, int x2, int y2) {
	F0(i, m + 1) F0(j, n + 1) v[i][j] = -1;
	v[x1][y1] = 1;
	deque<pii> Q; Q.push_back(pii(x1, y1));
	while (!Q.empty()) {
		int x = Q.front().first, y = Q.front().second; Q.pop_front();
		if (x == x2 && y == y2) break;
		F0(k, 4) {
			int xx = x + dx[k], yy = y + dy[k];
			int dir = ((xx>x) ^ (yy > y)) + 1;
			int xd = min(x, xx), yd = min(y, yy);
			if (d[xd][yd] == 0) {
				if (v[x][y] + 1 < v[xx][yy] || v[xx][yy] == -1) {
					v[xx][yy] = v[x][y] + 1;
					lastk[xx][yy] = k;
					Q.push_back(pii(xx, yy));
				}
			}
			else if (d[xd][yd] == dir) {
				if (v[x][y] + 1 < v[xx][yy] || v[xx][yy] == -1) {
					v[xx][yy] = v[x][y] + 1;
					lastk[xx][yy] = k;
					Q.push_back(pii(xx, yy));
				}
			}
		}
	}
	if (v[x2][y2] == -1) return 0;
	while (x2 != x1 || y2 != y1) {
		int k = lastk[x2][y2];
		int xd = min(x2, x2 - dx[k]), yd = min(y2, y2 - dy[k]);
		d[xd][yd] = ((dx[k] > 0) ^ (dy[k] > 0)) + 1;
		x2 -= dx[k];
		y2 -= dy[k];
	}
	return 1;
}

int doit(int i, int j) {
	int ret = single(X1[i], Y1[i], X2[j], Y2[j]);
	ret &= single(X2[i], Y2[i], X1[j], Y1[j]);
	return ret;
}

void solve() {
	vector< pair<int, pii> > V;
	F0(i, k) if (i < connect[i]) {
		pii p = make_pair(i, connect[i]);
		int dist = connect[i] - i;

		F0(j, k) if (j > i && j < connect[i] && !(connect[j] > i && connect[j] < connect[i])) {
			nosol = 1;
			return;
		}

		if (dist % 2 == 0) {
			nosol = 1;
			return;
		}
		dist = min(dist, k - dist);
		V.push_back(make_pair(dist, p));
	}
	sort(V.begin(), V.end());
	for (auto x : V) {
		if (!doit(x.second.first, x.second.second)) {
			nosol = 1;
			break;
		}

		/*
		cout << x.second.first << " " << x.second.second << endl;
		F0(i, m) {
			F0(j, n) {
				if (d[i][j] == 1) cout << "\\";
				else if (d[i][j] == 2) cout << "/";
				else if (d[i][j] == 0) cout << "?";
			}
			cout << endl;
		}*/
	}
}

int main() {
    //freopen("x.in", "r", stdin);

	freopen("C-small-attempt4.in", "r", stdin);
	freopen("C-small-attempt4.out", "w", stdout);

	//freopen("C-large.in", "r", stdin);
	//freopen("C-large.out", "w", stdout);

	int tt, tn; cin >> tn;

	F1(tt,tn) {
		//cerr << tt << endl;
		cin >> m >> n;
		F0(i, m) F0(j, n) d[i][j] = 0;
		printf("Case #%d: \n", tt);

		k = 0;
		F0(i, n) add(0, i, 0, i + 1);
		F0(i, m) add(i, n, i + 1, n);
		for (int i = n - 1; i >= 0; i--) add(m, i + 1, m, i);
		for (int i = m - 1; i >= 0; i--) add(i + 1, 0, i, 0);

		F0(it, k / 2) {
			cin >> i >> j; i--; j--;
			connect[i] = j; connect[j] = i;
		}

		nosol = 0;
		solve();
		if (nosol) {
			cout << "IMPOSSIBLE" << endl;
		}
		else {

			F0(i, m) F0(j, n) if (d[i][j] == 0) d[i][j] = 2;



			F0(i, m) {
				F0(j, n) {
					if (d[i][j] == 1) cout << "\\";
					else if (d[i][j] == 2) cout << "/";
					else if (d[i][j] == 0) cout << "/";
				}
				cout << endl;
			}
		}

	}
	return 0;
}
