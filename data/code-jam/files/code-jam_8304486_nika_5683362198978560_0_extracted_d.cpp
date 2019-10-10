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
const int MOD = 1000000007;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

int i, j, k, m, n, l, v, x[202], y[202], c[202];
char st[20005];
ll a[201][201], b[201][201];

int main() {
    //freopen("x.in", "r", stdin);

	freopen("D-small-attempt0.in", "r", stdin);
	freopen("D-small-attempt0.out", "w", stdout);

	//freopen("D-large.in", "r", stdin);
	//freopen("D-large.out", "w", stdout);

	int tt, tn; cin >> tn;

	F1(tt,tn) {
		//cerr << tt << endl;

		ll D;
		cin >> m >> n >> k >> D;

		F0(i, m) F0(j, n) {
			a[i][j] = 1e18;
		}
		F0(u, k) {
			cin >> x[u] >> y[u] >> c[u];
			x[u]--;
			y[u]--;

			F0(i, m) F0(j, n) a[i][j] = min(a[i][j], c[u] + (abs(x[u] - i) + abs(y[u] - j)) * D);
		}
		int good = 1;
		F0(u, k) {
			if (c[u] != a[x[u]][y[u]]) good = 0;
		}


		ll ans = 0;
		F0(i, m) F0(j, n) ans = (ans + a[i][j]) % MOD;

		if (good) {
			printf("Case #%d: %d\n", tt, (int)ans);
		}
		else {
			printf("Case #%d: IMPOSSIBLE\n", tt);
		}
	}
	return 0;
}
