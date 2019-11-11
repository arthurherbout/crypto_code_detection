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

ll m;
ll n;
vector<pair<ll, int> > vv;
vector<ll> v;

double getans(int n) {
	ll r = m;
	if (n < sz(vv))
		r = vv[n].fi;
	ll c = 0;
	ll s = 0;
	rep(i, n) {
		c += vv[i].se;
		s += vv[i].fi * vv[i].se;
	}
	ll need = c * vv[n - 1].fi - s;
	if (need > m)
		re 0;
	ll t = m - need;
	t /= c;
	re 0;
}

/*

double getans2(ll x) {
	ll s = 0;
	ll c = 0;
	vector<ll> v1;
	rep(i, sz(v))
	if (v[i] <= x) {
		s += x - v[i];
		v1.pb(x - v[i]);
		c++;
	}

	if (s > m)
		re 0;

	sort(all(v1));
	reverse(all(v1));

	double ans = 0;
	for (int i = 1; i <= sz(v1); i++) {
		if (s > m - (sz(v1) - i))
			continue;
		double p = 0;
		rep(j, i) {
			p += v1[j] * 36 / (double) i;
		}
		ans = max(ans, p - s - (sz(v1) - i));
	}
	re ans;
}*/

double getans(int l, int r) {
	ll s = 0;
	if (r < sz(v) && v[r] == v[r - 1])
		re 0;
	ll c = v[r - 1];
	ll s1 = 0;
	rep(i, r) {
		s += c - v[i];
		if (i < l)
			s1 += c - v[i];
	}
	s += r - l;
	//cout << s1 << endl;

	if (s > m)
		re 0;
	double ans = s1 * 36 / (double) l - s;
	//cout << ans << endl;
	ll o = (m - s) / r;
	if (r < sz(v) && v[r] <= o + c)
		o = v[r] - 1 - c;
	if (36 * o > r * o)
		ans += 36 * o - r * o;
	re ans;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tc;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		cin >> m >> n;
		v.clear();
		rep(i, n) {
			ll x;
			cin >> x;
			v.pb(x);
		}
		rep(i, 37 - n)
			v.pb(0);
		sort(all(v));
		/*vv.clear();
		rep(i, sz(v)) {
			if (i == 0 || v[i] != v[i - 1]) {
				vv.pb(mp(v[i], count(all(v), v[i])));
			}
		}*/

		//cout << getans(33, 35) << endl;
		//re 0;

		double ans = 0;
		for (int l = 1; l <= 37; l++)
			for (int r = l; r <= 37; r++)
				ans = max(ans, getans(l, r));
		/*for (int i = 1; i <= 2000; i++)
			ans = max(ans, getans2(i));*/

		/*
		rep(i, sz(vv))
			ans = max(ans, getans(i + 1));*/
		printf("%.10lf", ans);
		cout << endl;
	}



	re 0;
}

