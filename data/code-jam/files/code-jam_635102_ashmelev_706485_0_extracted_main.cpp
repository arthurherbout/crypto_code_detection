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
#include <cstdio>
#include <cstdlib>

#define mp make_pair
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int)((x).size()))
#define sqr(x) ((x)*(x))
#define For(i,a,b) for(int i = (a); i < (b); i++)
#define rep(i,n) For(i,0,n)
#define rrep(i,n) for (int i = ((n) - 1); i >= 0; i--)
#define re return
#define fi first
#define se second
#define y0 y47847892
#define y1 y47824262
#define fill(x, val) memset(x, val, sizeof(x))

using namespace std;

typedef vector<int> vi;
typedef vector<string> vs;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<vi> vvi;
typedef long long ll;

#pragma comment(linker, "/STACK:16777216")

template<class T> T abs(T x) { return x > 0 ? x : -x;}

int n;
int m;
int matr[110][110], newmatr[110][110];

int check() {
	rep(i, n) rep(j, n)
			if (matr[i][j]) re 1;
	re 0;
}

int good1(int x, int y) {
	re x > 0 && y > 0 && matr[x][y - 1] && matr[x - 1][y];
}

int good2(int x, int y) {
	re (x > 0 && matr[x - 1][y]) || (y > 0 && matr[x][y - 1]);
}

void update() {
	rep(i, n + 1) rep(j, n + 1) {
		newmatr[i][j] = matr[i][j];
		if (!matr[i][j] && good1(i, j))
			newmatr[i][j] = 1;
		if (matr[i][j] && !good2(i, j))
			newmatr[i][j] = 0;
	}

	memcpy(matr, newmatr, sizeof(matr));

}

int main() {

#ifndef ONLINE_JUDGE
	freopen("test.in", "r", stdin);
	freopen("test.out", "w", stdout);
#endif

	int tc;
	cin >> tc;

	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		n = 100;
		fill(matr, 0);
		cin >> m;
		rep(i, m) {
			int l1, l2, r1, r2;
			cin >> l1 >> r1 >> l2 >> r2;
			l1--;
			r1--;
			l2--;
			r2--;
			for (int x = l1; x <= l2; x++)
				for (int y = r1; y <= r2; y++)
					matr[y][x] = 1;
		}

		int ans = 0;
		while (check()) {
			/*cout << endl;
			rep(i, 7) {
				rep(j, 7)
						cout << matr[i][j] << ' ';
				cout << endl;
			}*/
			ans++;
			update();
		}
		cout << ans << endl;
	}

	return 0;
}
