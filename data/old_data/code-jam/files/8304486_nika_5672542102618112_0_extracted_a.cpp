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
#define CL(a,x) memset(x, a, sizeof(x));
const int MOD = 1000002013;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

int i, j, k, m, n, l, sn, f[10];
char st[20005];
int ans;
string s, t;

int d[10];

string nexts(string s) {
	string ret(n, '0');
	F0(i, 10) d[i] = 0;
	for (char c : s) d[c - '0']++;
	F0(i, n) ret[i] = '0' + d[i + 1];
	return ret;
}

void calc(string s) {
	F0(i, 10) d[i] = 0;
	ll ret = f[n];
	int all = n;
	for (char c : s) {
		int k = c - '0';
		ret /= f[k];
		all -= k;
	}
	ans += ret / f[all];
}

int good(string u) {
	if (t == s) return 1;
	while (u != t) {
		if (u == s) {
			return 1;
		}
		u = nexts(u);
	}
	return 0;
}

void go(string& p, int at, int sum) {
	if (at == n) {
		if (!sum) return;

		if (good(p)) {
			calc(p);
		}
		return;
	}
	for (int a = 0; a <= n; a++) if (a + sum <= n) {
		p[at] = a + '0';
		go(p, at + 1, sum + a);
		p[at] = 0;
	}
}

int main() {
    //freopen("x.in", "r", stdin);

	freopen("A-small-attempt1.in", "r", stdin);
	freopen("A-small-attempt1.out", "w", stdout);

	//freopen("A-large.in", "r", stdin);
	//freopen("A-large.out", "w", stdout);

	int tt, tn; cin >> tn;

	f[0] = 1;
	F1(i, 10) f[i] = f[i - 1] * i;

	F1(tt,tn) {
		//cerr << tt << endl;
		cin >> s;
		n = SZ(s);
		ans = 0;

		t = string(n, '0');
		t[0] = '1';

		string p(n, '0');
		go(p, 0, 0);
	
		if (s != t) ans++;
		printf("Case #%d: %d\n", tt, ans);
	}
	return 0;
}
