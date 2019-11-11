#include <iostream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <bitset>
#include <deque>
#include <map>
#include <stack>
#include <sstream>
#include <cstring>

#define mp make_pair
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int)((x).size()))
#define sqr(x) ((x)*(x))
#define For(i,a,b) for(int i = (a); i < (b); i++)
#define rep(i,n) For(i,0,n)
#define re return
#define fi first
#define se second
#define y0 y47847892
#define y1 y47824262
#define fill(x, val) memset(x, val, sizeof(x))

using namespace std;

typedef vector<int> vi;
typedef long long ll;
typedef vector<string> vs;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<vi> vvi;

template<class T> T abs(T x) { return x > 0 ? x : -x;}

int n;
int m;

int matr[100][100];

queue<pair<ii, int> > q;

int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, -1, 0, 1};
int was[50][50][1000];
int ind[50][50][1000];
char si[50][50][1000];
pair<ii, int> prev[50][50][1000];
int glob;

int good(int x, int y) {
	re x >= 0 && y >= 0 && x < n && y < n;
}

void parse(pair<ii, int> z) {
	int x = z.fi.fi, y = z.fi.se, val = z.se;
	ind[x][y][val + 500] = glob++;

	//cout << x << ' ' << y << ' ' << val << endl;

	rep(d, 10) {

	rep(i, 4)
	rep(j, 4){
		int nx = x + dx[i] + dx[j];
		int ny = y + dy[i] + dy[j];
		if (good(nx, ny) && good(x + dx[i], y + dy[i])) {
			char c = matr[x + dx[i]][y + dy[i]];
			int val1 = matr[nx][ny] - 48;
			if (val1 == d)
			if (c == '+') {
				int nval = val + val1;
				if (nval < 500)
				if (!was[nx][ny][nval + 500]) {
					was[nx][ny][nval + 500] = 1;
					prev[nx][ny][nval + 500] = mp(mp(x, y), val + 500);
					si[nx][ny][nval + 500] = '+';
					q.push(mp(mp(nx, ny), nval));
				}
			}
		}
	}

	}

	rep(d, 10) {

	rep(i, 4)
	rep(j, 4) {
		int nx = x + dx[i] + dx[j];
		int ny = y + dy[i] + dy[j];
		if (good(nx, ny)) {
			char c = matr[x + dx[i]][y + dy[i]];
			int val1 = matr[nx][ny] - 48;
			if (val1 == d)
			if (c == '-') {
				int nval = val - val1;
				if (nval >= -500)
				if (!was[nx][ny][nval + 500]) {
					was[nx][ny][nval + 500] = 1;
					prev[nx][ny][nval + 500] = mp(mp(x, y), val + 500);
					si[nx][ny][nval + 500] = '-';
					q.push(mp(mp(nx, ny), nval));
				}
			}
		}
	}

	}
}

int main() {

#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif

	int tCount;
	cin >> tCount;
	rep(test, tCount) {
		cout << "Case #" << test + 1 << ":" << endl;

		int qq;
		cin >> n >> qq;
		rep(i, n) {
			string s;
			cin >> s;
			rep(j, n)
				matr[i][j] = s[j];
		}

		memset(was, 0, sizeof(was));
		rep(d, 10)
		rep(i, n) rep(j, n)
		if (matr[i][j] == d + 48) {
			q.push(mp(mp(i, j), d));
			prev[i][j][d + 500] = mp(mp(-1, -1), -1);
			was[i][j][d + 500] = 1;
		}

		glob = 0;
		while (!q.empty()) {
			parse(q.front());
			q.pop();
		}

		rep(i, qq) {
			int tmp;
			cin >> tmp;

			int best = 1000000000, bx = -1, by = -1;


			string gans = "";
			rep(i, n) rep(j, n)
			if (was[i][j][tmp + 500]) {
/*				if (best > ind[i][j][tmp + 500]) {
					bx = i;
					by = j;
					best = ind[i][j][tmp + 500];
				}
			}

			if (bx == -1) {
				cout << "BAD";
				re 0;
			}

			int i = bx, j = by;
				 {*/
					pair<ii, int> p = mp(mp(i, j), tmp + 500);
					string ans;
					//cout << "z" << endl;
					while (p.fi.fi != -1) {
						ans += (matr[p.fi.fi][p.fi.se]);
						if (prev[p.fi.fi][p.fi.se][p.se].fi.fi != -1)
							ans +=(si[p.fi.fi][p.fi.se][p.se]);
						p = prev[p.fi.fi][p.fi.se][p.se];
					}
					int sum = 0;
					reverse(all(ans));

					if (sz(gans) == 0 || sz(gans) > sz(ans) || (sz(ans) == sz(gans) && ans < gans))
						gans = ans;
					//cout << gans;
					if (0)
					rep(i, sz(ans)) {
						if (i == 0)
							sum = ans[i] - 48;
						else
							if (i % 2 == 0) {
								if (ans[i - 1] == '-')
									sum -= (ans[i] - 48);
								else
									sum += (ans[i] - 48);
							}

						cout << ans[i];
						/*ii a = ans[i];
						cout << (char)matr[a.fi][a.se];
						if (i < sz(ans) - 1) {
							ii b = ans[i + 1];
							int x = a.fi + b.fi;
							int y = a.se + b.se;
							x /= 2;
							y /= 2;
							cout << (char)matr[x][y];
						}*/
					}
					/*
					if (sum != tmp) {
						cout << "BAD";
						re 0;
					}*/
					//cout << endl;
				}
				cout << gans << endl;
		}

	}

	return 0;
}
