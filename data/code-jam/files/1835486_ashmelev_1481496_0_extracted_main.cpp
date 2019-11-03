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

char get2(char c) {
	if (c == 'o')
		re 'z' + 1;
	if (c == 'i')
		re 'z' + 2;
	if (c == 'e')
		re 'z' + 3;
	if (c == 'a')
		re 'z' + 4;
	if (c == 's')
		re 'z' + 5;
	if (c == 't')
		re 'z' + 6;
	if (c == 'b')
		re 'z' + 7;
	if (c == 'g')
		re 'z' + 8;
	re 0;
}

vs get(string s) {
	vs ans;
	rep(i, sz(s) - 1) {
		char c1 = s[i];
		char c2 = s[i + 1];
		vector<char> v1, v2;
		v1.pb(c1);
		if (get2(c1) != 0)
			v1.pb(get2(c1));
		v2.pb(c2);
		if (get2(c2) != 0)
			v2.pb(get2(c2));
		rep(o1, sz(v1))
		rep(o2, sz(v2)) {
			string ss = "";
			ss += v1[o1];
			ss += v2[o2];
			ans.pb(ss);
		}
	}
	re ans;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tc;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		int k;
		cin >> k;
		string s;
		cin >> s;
		vs v = get(s);
		sort(all(v));
		unq(v);

		int c1[40], c2[40];
		fill(c1, 0);
		fill(c2, 0);
		rep(i, sz(v)) {
			int a = v[i][0] - 'a';
			int b = v[i][1] - 'a';
			//cout << a << ' ' << b << endl;
			if (a < 0)
				a += 256;
			if (b < 0)
				b += 256;
			c1[a]++;
			c2[b]++;
		}

		//rep(i, 40)
		//cout << c1[i] << ' ' << c2[i] << endl;

		int d = 0;
		rep(i, 40)
			d += abs(c1[i] - c2[i]);
		if (d > 0)
			d -= 2;
		d /= 2;
		cout << sz(v) + d + 1 << endl;
	}

	re 0;
}
