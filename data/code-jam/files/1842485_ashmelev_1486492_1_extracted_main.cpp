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

int h[5000];
int next[5000];

void go(int l, int r, int dx) {
	vi v;
	int c = l;
	while (c != r) {
		v.pb(c);
		c = next[c];
	}
	v.pb(r);

	int ch = h[r];
	rep(i, sz(v) - 1) {
		int a = v[i];
		h[a] = ch - dx * (r - a);
	}

	rep(i, sz(v) - 1) {
		int a = v[i];
		int b = v[i + 1];
		if (b - a > 1)
			go(a + 1, b, dx + 1);
	}
}

vii v;

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tc;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		cin >> n;

		v.clear();
		rep(i, n - 1) {
			int x;
			cin >> x;
			x--;
			next[i] = x;
			v.pb(mp(i, x));
		}

		int gg = 1;
		rep(i, sz(v)) rep(j, i) {
			int a = v[j].fi;
			int b = v[j].se;
			int c = v[i].fi;
			int d = v[i].se;
			if (c < b && d > b) {
				gg = 0;
			}
		}

		if (!gg) {
			cout << "Impossible" << endl;
			continue;
		}

		h[n - 1] = 50000000;
		go(0, n - 1, 1);

		rep(i, n)
		cout << h[i] << ' ';
		cout << endl;
	}

	re 0;
}
