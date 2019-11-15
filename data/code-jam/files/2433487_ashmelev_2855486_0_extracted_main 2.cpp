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

int p[10];
int sp[10];

ll x[100], y[100];

ll get(ll x1, ll y1, ll x2, ll y2) {
	re x1 * y2 - x2 * y1;
}

int inside(int x1, int y1, int x2, int y2, int x3, int y3) {
	if (x1 > x2  && x1 > x3)
		re 0;
	if (x1 < x2 && x1 < x3)
		re 0;
	if (y1 > y2 && y1 > y3)
		re 0;
	if (y1 < y2 && y1 < y3)
		re 0;
	re 1;
}

int check1(int x1, int y1, int x2, int y2, int x3, int y3) {
	if (get(x1 - x2, y1 - y2, x1 - x3, y1 - y3) != 0)
		re 1;
	if (inside(x1, y1, x2, y2, x3, y3))
		re 0;
	if (inside(x3, y3, x1, y1, x2, y2))
		re 0;
	re 1;
}

int check1(int i, int j) {
	re check1(x[p[i]], y[p[i]], x[p[j]], y[p[j]], x[p[(j +1) % n]], y[p[(j + 1) % n]]);
}

int inter(ll x1, ll y1, ll x2, ll y2, ll x3, ll y3, ll x4, ll y4) {
	ll d = get(x2 - x1, y2 - y1, x3 - x4, y3 - y4);
	ll d1 = get(x3 - x1, y3 - y1, x3 - x4, y3 - y4);
	ll d2 = get(x2 - x1, y2 - y1, x3 - x1, y3 - y1);

	if (d < 0) {
		d *= -1;
		d1 *= -1;
		d2 *= -1;
	}

	if (d1 < 0 || d1 > d || d2 < 0 || d2 > d)
		re 0;
	re 1;
}

int check2(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
	ll d = get(x1 - x2, y1 - y2, x3 - x4, y3 - y4);
	if (d == 0) {
		if (inside(x1, y1, x3, y3, x4, y4))
			re 0;
		if (inside(x2, y2, x3, y3, x4, y4))
			re 0;
		if (inside(x3, y3, x1, y1, x2, y2))
			re 0;
		if (inside(x4, y4, x1, y1, x2, y2))
			re 0;
		re 1;
	}

	re !inter(x1, y1, x2, y2, x3, y3, x4, y4);
}

int check2(int i, int j) {
	if ((j + 1) % n == i)
		re 1;
	re check2(x[p[i]], y[p[i]], x[p[i + 1]], y[p[i + 1]], x[p[j]], y[p[j]], x[p[j + 1]], y[p[j + 1]]);
}

ll get() {
	/*rep(i, n)
		cout << p[i] << ' ';
	cout << endl;*/
	rep(i, n) {
		if (!check1(i, (i + 1) % n)) {
			//cout << "bad " << i << endl;
			re 0;
		}
		for (int j = i + 2; j < n; j++)
			if (!check2(i, j)) {
				//cout << "bad " << i << ' ' << j << endl;
				re 0;
			}
	}
	ll s = 0;
	rep(i, n)
		s += (y[p[i]] + y[p[(i + 1) % n]]) * (x[p[(i + 1) % n]] - x[p[i]]);
	//cout << "ok " << s << endl;
	re abs(s);
}


int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tc;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		cin >> n;
		rep(i, n)
		p[i] = i;
		rep(i, n)
		cin >> x[i] >> y[i];
		x[n] = x[0];
		y[n] = y[0];

		//p[2] = 4;
		//p[3] = 2;
		//p[4] = 3;

		//cout << get() << endl;
		//re 0;

		//cout << get() << endl;

		ll bs = -1;
		do {
			ll s = get();
			//cout << s << endl;
			if (s > bs) {
				bs = s;
				//cout << "s = " << s << endl;
				memcpy(sp, p, sizeof(p));
			}
		}
		while (next_permutation(p + 1, p + n));
		rep(i, n)
		cout << sp[i] << ' ';
		cout << endl;
	}



	re 0;
}

