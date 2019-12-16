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

vector<pair<ii, ii> > v;


int dd[10][10];
int st[10], end[10];


int diff(pair<ii, ii> a, pair<ii, ii> b) {
	int dx = 0;
	int dy = 0;
	if (b.fi.fi > a.se.fi)
		dx = b.fi.fi - a.se.fi;
	if (a.fi.fi > b.se.fi)
		dx = a.fi.fi - b.se.fi;
	if (b.fi.se > a.se.se)
		dy = b.fi.se - a.se.se;
	if (a.fi.se > b.se.se)
		dy = a.fi.se - b.se.se;

/*	cout << a.fi.fi << ' ' << a.fi.se << ' ' << a.se.fi << ' ' <<  a.se.se << " -> " <<
			b.fi.fi << ' ' << b.fi.se << ' ' << b.se.fi << ' ' <<  b.se.se << endl;
	cout << dx << ' ' << dy << endl;*/

	re max(0, max(dx, dy) - 1);
}

int check(vi o) {
	int ans = v[o[0]].fi.se + (m - v[o.back()].se.se - 1);
	rep(i, sz(o) - 1) {
		ans += dd[o[i]][o[i+1]];//diff(v[o[i]], v[o[i + 1]]);
	}
	re ans;
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

		int k;
		cin >> n >> m >> k;
		v.clear();
		rep(i, k) {
			int x1, y1, x2, y2;
			cin >> x1 >> y1 >> x2 >> y2;
			v.pb(mp(mp(x1, y1), mp(x2, y2)));
			st[i] = y1;
			end[i] = m - y2 - 1;
		}

		/*vi v;
		v.pb(0);
		v.pb(1);
		cout << check(v) << endl;
		re 0;*/

		int id[10];

		rep(i, 10)
			id[i] = i;

		rep(i, k) rep(j, k)
			dd[i][j] = diff(v[i], v[j]);

		int ans = m;

		if (k > 0) {
		do {
			vi v;
			int tmp = st[id[0]];
			ans = min(ans, tmp + end[id[0]]);
			for (int i = 1; i < k; i++) {
				tmp += dd[id[i]][id[i - 1]];
				if (tmp > ans)
					break;
				ans = min(ans, tmp + end[id[i]]);
			}
		}
		while (next_permutation(id, id + k));
		}
		cout << ans << endl;
	}


	re 0;
}

