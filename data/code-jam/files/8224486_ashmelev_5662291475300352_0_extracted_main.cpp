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

vii v;

int table[500][100];

int getans(int p, int id) {
	int &ans = table[p][id];
	if (ans != -1)
		re ans;
	ans = sz(v);
	re ans;
}

int getcol(ll x, ll v, ll t) {
	ll col = t / v;
	x = 360 - x;
	if (x > col)
		re 0;
	col -= x;
	re 1 + col / 360;
}

int getans(ll t) {
	ll ans = 0;
	rep(i, sz(v)) {
		ll c = getcol(v[i].fi, v[i].se, t);
		if (c == 0)
			ans++;
		if (c > 1)
			ans += c - 1;
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
	rep(te, tc) {
		cout << "Case #" << te + 1 << ": ";
		v.clear();

		int k;
		cin >> k;
		rep(i, k) {
			int p, t, s;
			cin >> p >> t >> s;
			rep(i, t) {
				v.pb(mp(p, (s + i) * 2));
			}
		}

		/*rep(i, sz(v))
			cout << v[i].fi << ' ' << v[i].se << endl;


		fill(table, -1);
		cout << getans(0, 0) << endl;*/
		n = sz(v);

		vector<ll> tt;
		rep(i, n)
			tt.pb((360 - v[i].fi) * (ll)v[i].se);

		sort(all(tt));
		/*
		cout << endl;
		rep(i, sz(tt))
			cout << tt[i] + 1 << endl;
		cout << getans(tt[0] + 1) << endl;
		re 0;*/

		int ans = getans(tt[0] - 1);
		rep(i, n)
			ans = min(ans, getans(tt[i] + 1));
		cout << ans << endl;
	}

	re 0;
}
