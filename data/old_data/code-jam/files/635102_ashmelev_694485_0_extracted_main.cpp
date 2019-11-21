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

template<class T> T abs(T x) { return x > 0 ? x : -x;}

int n;
int m;

int matr[100][100];

int bob[104][104];


int check(int n) {
	rep(i, n) rep(j, i)
		if (bob[i][j] != -1 && bob[j][i] != -1 && bob[i][j] != bob[j][i])
			re 0;

	rep(j, n) rep(i, n)
		if (bob[i][j] != -1 && bob[n - j - 1][n - i - 1] != -1 && bob[i][j] != bob[n - j - 1][n - i - 1])
			re 0;

	re 1;
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

		cin >> n;
		//cout << endl << endl;
		//cout << "n = " << n << endl;
		rep(i, n) {
			rep(j, i + 1)
				cin >> matr[i - j][j];
		}

		rep(i, n - 1) {
			rep(j, n - i - 1)
				cin >> matr[n - j - 1][i + j + 1];
		}

/*		rep(i, n) {
			rep(j, n)
				cout << matr[i][j] << ' ';
			cout << endl;
		}*/

		rep(d, 55) {
			int f = 0;
			rep(dx, d + 1) rep(dy, d + 1) {
				fill(bob, -1);
				if (f)
					break;
				rep(i, n) rep(j, n)
					bob[i + dx][j + dy] = matr[i][j];

				if (0)
				if (d == 0) {
					cout << check(n) << endl;
					rep(i, n) {
						rep(j, n)
							cout << bob[i][j] << ' ';
						cout << endl;
					}

				}

				if (check(d + n)) {
					f = 1;
					break;
				}
			}

			if (f) {
				cout << sqr(d + n) - sqr(n) << endl;
				break;
			}
		}
	}

	return 0;
}
