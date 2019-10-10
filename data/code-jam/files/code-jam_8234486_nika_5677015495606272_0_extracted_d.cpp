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
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

int i, j, k, m, n, l;
int a[6][6];
int ans;

int dx[] = { -1, 0, 1, 0 };
int dy[] = { 0, 1, 0, -1 };

set< vector<int> > S;

void go(int x, int y) {
	// verify
	for (int z = 3; z >= 1; z--)
	F0(i, m) F0(j, n) if (a[i][j] == z) {
		int cnt = 0, c = 0;
		F0(k, 4) {
			int x2 = i + dx[k]; if (x2 < 0 || x2 >= m) continue;
			int y2 = (j + dy[k] + n) % n;
			if (a[x2][y2] == 0 || a[x2][y2] == z) cnt++;
			if (a[x2][y2] == z) c++;
		}
		if (cnt < z) return;
		if (c > z) return;
	}

	if (x == m) {
		/*
		cout << endl;
		F0(i, m) {
			F0(j, n) {
				cout << a[i][j] << " ";
			}
			cout << endl;
		}
		*/
		vector<int> ret;
		F0(i, m) F0(j, n) ret.push_back(a[i][j]);
		if (!S.count(ret)) {
			for (int jj = 0; jj < n; jj++) {
				vector<int> r;
				F0(i, m) F0(j, n) {
					r.push_back(a[i][(j + jj) % n]);
				}
				S.insert(r);
			}
			ans++;
		}
		return;
	}
	if (y == n) { go(x + 1, 0); return; }
	if (a[x][y] != 0) { go(x, y + 1); return; }


	// 1's
	/*
	for (int k = 0; k < 4; k++) {
		int x2 = i + dx[k]; if (x2 < 0 || x2 >= m) continue;
		int y2 = (y + dy[k] + n) % n;
		if (a[x2][y2] == 0) {
			a[x][y] = 1;
			a[x2][y2] = 1;
			go(x, y + 1);
			a[x][y] = 0;
			a[x2][y2] = 0;
		}
	}*/

	a[x][y] = 1; go(x, y + 1); a[x][y] = 0;
	a[x][y] = 2; go(x, y + 1); a[x][y] = 0;
	a[x][y] = 3; go(x, y + 1); a[x][y] = 0;
}

int pre[7][7];

int main() {
    //freopen("x.in", "r", stdin);

	freopen("D-small-attempt1.in", "r", stdin);
	freopen("D-small-attempt1.out", "w", stdout);

	//freopen("D-large.in", "r", stdin);
	//freopen("D-large.out", "w", stdout);

	for (m = 2; m <= 6; m++) {
		for (n = 3; n <= 6; n++) {
			ans = 0;
			go(0, 0);
			pre[m][n] = ans;
			//cout << ans << " ";
		}
		//cout << endl;
	}

	int tt, tn; cin >> tn;

	F1(tt,tn) {
		//cerr << tt << endl;
		printf("Case #%d: ", tt);

		cin >> m >> n;

		ans = pre[m][n];
		cout << ans;
		cout << endl;
	}
	return 0;
}
