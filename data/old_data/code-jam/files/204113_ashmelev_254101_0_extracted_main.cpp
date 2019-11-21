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

string s[60];

int ans[50][50][51][51];

bool l1(pair<ii, ii> a, pair<ii, ii> b) {
/*	if (ans[a.fi.fi][a.fi.se][a.se.fi][a.se.se] == -1)
		while (1);
	if (ans[b.fi.fi][b.fi.se][b.se.fi][b.se.se] == -1)
		while (1);*/
	if (ans[a.fi.fi][a.fi.se][a.se.fi][a.se.se] != ans[b.fi.fi][b.fi.se][b.se.fi][b.se.se])
		re ans[a.fi.fi][a.fi.se][a.se.fi][a.se.se] < ans[b.fi.fi][b.fi.se][b.se.fi][b.se.se];

	re a < b;
}

set<pair<ii, ii>, bool (*) (pair<ii, ii>, pair<ii, ii>)> ss(l1);
int res;
int f;

int good(int x, int y, int l, int r) {
	if (x >= 0 && y >= 0 && x < n && y < m)
		if (s[x][y] == '.' || (y >= l && y <= r))
			re 1;
	re 0;
}

void parse(pair<ii, ii> a) {
	int x = a.fi.fi, y = a.fi.se;
	int l = a.se.fi, r = a.se.se;
	int d = ans[x][y][l][r];

	//cout << x << ' ' << y << ' ' << d << endl;

	//cout << d << endl;

	if (x == n - 1) {
		res = min(res, d);
		re;
	}

	int nx, ny;

	ny = y + 1;
	nx = x;
	if (good(nx, ny, l, r)) {
		nx = x;
		while (nx < n - 1 && s[nx + 1][ny] == '.')
			nx++;
		if (nx - x <= f) {
			int g = ans[nx][ny][50][50];

			if (g > d) {
				ss.erase(mp(mp(nx, ny), mp(50, 50)));
				ans[nx][ny][50][50] = d;
				ss.insert(mp(mp(nx, ny), mp(50, 50)));
			}
			if (ans[nx][ny][50][50] == -1) {
				ans[nx][ny][50][50] = d;
				ss.insert(mp(mp(nx, ny), mp(50, 50)));
			}
		}
	}

	ny = y - 1;
	nx = x;
	if (good(nx, ny, l, r)) {
		nx = x;
		while (nx < n - 1 && s[nx + 1][ny] == '.')
			nx++;
		if (nx - x <= f) {
			int g = ans[nx][ny][50][50];

			if (g > d) {
				ss.erase(mp(mp(nx, ny), mp(50, 50)));
				ans[nx][ny][50][50] = d;
				ss.insert(mp(mp(nx, ny), mp(50, 50)));
			}

			if (g == -1) {
				ans[nx][ny][50][50] = d;
				ss.insert(mp(mp(nx, ny), mp(50, 50)));
			}

		}
	}


	int z = y;
	while (z < n - 1 && good(x, z + 1, l, r) && s[x + 1][z + 1] == '#')
		z++;
	for (int u = 1; u + y <= z; u++) {
	nx = x;
	ny = y + 1;
	if (good(nx, ny, l, r)) {
		if (s[nx + 1][ny] == '#'){
		nx = x + 1;
		while (nx < n - 1 && s[nx + 1][ny] == '.')
			nx++;
		if (nx - x <= f) {

			/*if (nx != x + 1)
				u = 1;*/
			if (u > 1 && nx != x + 1)
				continue;

			int g = ans[nx][ny][y + 1][y + u];

			if (g > d + u) {
				ss.erase(mp(mp(nx, ny), mp(y + 1, y + u)));
				ans[nx][ny][y + 1][y + u] = d + u;
				ss.insert(mp(mp(nx, ny), mp(y + 1, y + u)));
			}

			if (ans[nx][ny][y + 1][y + u] == -1) {
				ans[nx][ny][y + 1][y + u] = d + u;
				ss.insert(mp(mp(nx, ny), mp(y + 1, y + u)));
			}
		}
		}
	}
	}


	z = y;
	while (z > 0 && good(x, z - 1, l, r) && s[x + 1][z - 1] == '#')
		z--;
	for (int u = 1; y - u >= z; u++) {
	nx = x;
	ny = y - 1;
	if (good(nx, ny, l, r)) {
		if (s[nx + 1][ny] == '#'){
		nx = x + 1;
		while (nx < n - 1 && s[nx + 1][ny] == '.')
			nx++;
		if (nx - x <= f) {

			/*if (nx != x + 1)
				u = 1;*/

			if (u > 1 && nx != x + 1)
				continue;

			int g = ans[nx][ny][y - u][y  - 1];

			if (g > d + u) {
				ss.erase(mp(mp(nx, ny), mp(y - u, y - 1)));
				ans[nx][ny][y - u][y - 1] = d + u;
				ss.insert(mp(mp(nx, ny), mp(y - u, y - 1)));
			}

			if (ans[nx][ny][y - u][y - 1] == -1) {
				ans[nx][ny][y - u][y - 1] = d + u;
				ss.insert(mp(mp(nx, ny), mp(y - u, y - 1)));
			}
		}
		}
	}
	}

/*
	z = y;
	while (z > 0 && s[x][z - 1] == '.' && s[x + 1][z - 1] == '#')
		z--;
	for (int u = 1; y - u >= z; u++) {
//	int u = 1;
	nx = x;
	ny = y - u;
	if (good(nx, ny)) {
		if (s[nx + 1][ny] == '#'){
		nx = x + 1;
		while (nx < n - 1&& s[nx + 1][ny] == '.')
			nx++;
		if (nx - x <= f) {
			if (ans[nx][ny] == -1) {
				ans[nx][ny] = d + u;
				ss.insert(mp(nx, ny));
			}
		}
		}
	}
	}*/
}

int main() {

#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif

	int tCount;
	cin >> tCount;
	rep(test, tCount) {
		cout << "Case #" << test + 1 << ":" << ' ';

		cin >> n >> m >> f;
		rep(i, n)
			cin >> s[i];

//		cout << n << ' ' << m << ' ' << endl;
//		cout << s[0] << endl;

		res = 10000;
		memset(ans, 255, sizeof(ans));
		ans[0][0][50][50] = 0;
		ss.insert(mp(mp(0, 0), mp(50, 50)));
		while (!ss.empty()) {
			pair<ii, ii> u = *ss.begin();
			ss.erase(u);
			parse(u);
		}

		if (res < 10000) {
			cout << "Yes " << res;
		}
		else
			cout << "No";


		cout << endl;
	}

	return 0;
}
