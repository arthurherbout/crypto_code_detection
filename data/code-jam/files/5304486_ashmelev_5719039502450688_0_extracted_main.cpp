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
#define next next239
#define prev prev239

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

template<class T> string toString(T x) {
	stringstream sout;
	sout << x;
	re sout.str();
}

int nint() {
	int x;
	scanf("%d", &x);
	re x;
}

const ll mod = 1000000000 + 7;

int m;
int n;
ll ans;

ll hd, ad, hk, ak, b, d, k;

ll getSingle(ll ch) {
	ll ans = k;
	rep(i, k - 1) {
		if (ch <= ak) {
			ans++;
			ch = hd - ak;
		}
		ch -= ak;
		if (ch <= 0)
			re 1e18;
	}

	re ans;
}

ll getMin(ll a, ll b, ll h) {
	ll ans = 1e18;
	ll o = 0;
	while (o <= ans) {
		ans = min(ans, o + (h + a - 1) / a);
		o++;
		a += b;
	}
	re ans;
}

int main() {
#ifdef LOCAL_BOBER
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif

	int tc = 1;
	cin >> tc;
	rep(tt, tc) {

		cout << "Case #" << tt + 1 << ": ";

		cin >> hd >> ad >> hk >> ak >> b >> d;

		int d1 = max(ak - d, 0ll);
		int d2 = max(d1 - d, 0ll);

		k = getMin(ad, b, hk);

		if (k == 1) {
			cout << 1 << endl;
			continue;
		}

		if (k == 2 && hd > ak) {
			cout << 2 << endl;
			continue;
		}

		if (d1 + d2 >= hd) {
			cout << "IMPOSSIBLE" << endl;
			continue;
		}


		ll ans = (ll)1e18;
		ll ch = hd;
		ll cur = 0;
		while (1) {
			if (cur + k >= ans)
				break;
			ans = min(ans, cur + getSingle(ch));
			if (ch <= max(ak - d, 0ll)) {
				cur++;
				ch = hd - ak;
			}
			else {
				cur++;
				ak -= d;
				ak = max(ak, 0ll);
				ch -= ak;
			}
		}
		cout << ans << endl;
	}


	re 0;
}










