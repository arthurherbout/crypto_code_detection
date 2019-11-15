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
ll l;
ll b[100];
int table[100][21001];
int big = 10000000;

int getans(int p, int len) {
	if (!len)
		re 0;
	if (p == n - 1) {
		if (len % b[p] == 0)
			re len / b[p];
		else
			re big;
	}
	int &ans = table[p][len];
	if (ans != -1)
		re ans;
	ans = getans(p + 1, len);
	if (b[p] <= len)
		ans = min(ans, 1 + getans(p, len - b[p]));
	re ans;
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

		cin >> l >> n;
		ll u = l - 20000;
		rep(i, n)
			cin >> b[i];
		sort(b, b + n);
		reverse(b, b + n);
		ll col = u / b[0];
		ll ans = col;
		l -= col * b[0];

		fill(table, -1);
		int ans1 = getans(0, l);

		if (ans1 < big)
			cout << ans + ans1 << endl;
		else
			cout << "IMPOSSIBLE" << endl;
	}

	return 0;
}
