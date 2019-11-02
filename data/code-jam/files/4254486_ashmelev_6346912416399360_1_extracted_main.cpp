#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <bitset>
#include <queue>
#include <stack>
#include <sstream>
#include <cstring>
#include <numeric>
#include <ctime>

#define re return
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int) (x).size())
#define rep(i, n) for (int i = 0; i < (n); i++)
#define rrep(i, n) for (int i = (n) - 1; i >= 0; i--)
#define y0 y32479
#define y1 y95874
#define fill(x, y) memset(x, y, sizeof(x))
#define sqr(x) ((x) * (x))
#define sqrt(x) sqrt(abs(x))
#define unq(x) (x.resize(unique(all(x)) - x.begin()))
#define spc(i,n) " \n"[i == n - 1]

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<string> vs;
typedef double D;
typedef long double LD;
typedef long long ll;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

template<class T> T abs(T x) { return x > 0 ? x : -x;}

int m;
int n;

int cl[1500], cr[1500], cs[1500];

int check(int d, int i, int ss) {
	int f = 1;
	ll sum = 0;
	ll dd = 0;
	rep(j, m) {
		int lx, rx;
		if (i == j) {
			lx = -cl[i];
			rx = -cl[i];
		}
		else {
			lx = -cl[j];
			rx = d - cr[j];
			if (rx < lx) {
				f = 0;
				break;
			}
		}
		sum += lx;
		dd += rx - lx;
	}
	if (f) {
		int g = 0;
		rep(i, dd + 1) {
			if ((sum + i) % m == ss) {
				g = 1;
				break;
			}
		}
		if (g) {
			//cout << i << ' ' << d << ' ' << sum << ' ' << dd << ' ' << s[0] << ' ' << m << ' ' <<endl;
			re 1;
		}
	}
	re 0;
}

int main() {
#ifdef LOCAL_BOBER
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif

	int tc;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		cin >> n >> m;
		int s[1500];

		rep(i, n - m + 1)
			cin >> s[i];

		if (n == m) {
			if (s[0] % n == 0) {
				cout << 0 << endl;
			}
			else
				cout << 1 << endl;
			continue;
		}

		fill(cl, 0);
		fill(cr, 0);
		fill(cs, 0);

		for (int i = 1; i < n - m + 1; i++) {
			int d = s[i] - s[i - 1];
			int p = (i - 1) % m;
			cs[p] += d;
			cl[p] = min(cl[p], cs[p]);
			cr[p] = max(cr[p], cs[p]);
		}

		//rep(i, m) {
		//	cout << i << ' ' << cl[i] << ' ' << cr[i] << endl;
		//}


		while (s[0] < 0)
			s[0] += m;
		s[0] %= m;

		int ans = 1000000000;

		rep(i, m) {
			int l = cr[i] - cl[i];
			int r = ans - 1;
			while (l <= r) {
				int d = (l + r) / 2;
				if (check(d, i, s[0])) {
					ans = d;
					r = d - 1;
				}
				else
					l = d + 1;
			}
			/*for (int d = cr[i] - cl[i]; d < ans; d++) {
				if (check(d, i, s[0])) {
					ans = d;
				}
			}*/
		}

		cout << ans << endl;
	}

	re 0;
}
