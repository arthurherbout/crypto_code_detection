#define _SECURE_SCL 0
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


typedef long long int64;
#define double long double
const int INF = (int) 1E9;
const int64 INF64 = (int64) 1E18;
const double EPS = 1E-9;
const double EPS_ANG = 1E-16;
const double PI = acos((double)0) * 2;

#define forn(i,n)  for (int i=0; i<int(n); ++i)
#define ford(i,n)  for (int i=int(n)-1; i>=0; --i)
#define all(a)  a.begin(), a.end()
#define fs  first
#define sc  second
#define mp  make_pair
#define pb  push_back


const int MAXN = 20;
const int dxy[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };


int n, m, k;


void read() {
	cin >> n >> m >> k;
}


bool val[MAXN][MAXN], u[MAXN][MAXN];

void dfs(int x, int y) {
	u[x][y] = true;
	forn(dir, 4) {
		int nx = x + dxy[dir][0];
		int ny = y + dxy[dir][1];
		if (0 <= nx && nx < n && 0 <= ny && ny < m)
			if (!u[nx][ny] && !val[nx][ny])
				dfs(nx, ny);
	}
}

void solve() {
	int ans = INF;
	forn(msk, 1 << (n * m)) {
		int bits = 0;
		forn(i, n * m) {
			bool cur = (msk & (1 << i)) != 0;
			val[i / m][i % m] = cur;
			if (cur)
				++bits;
		}
		if (bits >= ans)
			continue;

		forn(x, n)
			forn(y, m)
				u[x][y] = false;
		forn(x, n)
			forn(y, m)
				if (!val[x][y] && !u[x][y] && (x == 0 || x == n - 1 || y == 0 || y == m - 1))
					dfs(x, y);
		
		int cnt = 0;
		forn(x, n)
			forn(y, m)
				if (!u[x][y])
					++cnt;
		if (cnt == k)
			ans = bits;
	}
	cout << ans << endl;
}


int main() {
#ifdef SU2_PROJ
	freopen ("input.txt", "rt", stdin);
	freopen ("output.txt", "wt", stdout);
#endif

	int ts;
	cin >> ts;
	forn(tt, ts) {
		cerr << "Case #" << tt + 1 << "..." << endl;
		read();
		printf("Case #%d: ", tt + 1);
		solve();
	}
	cerr << "Finish." << endl;

	return 0;
}
