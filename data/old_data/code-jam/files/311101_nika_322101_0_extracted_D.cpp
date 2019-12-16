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
#define F0(i,n) for(i=0;i<n;i++)
#define F1(i,n) for(i=1;i<=n;i++)
const int inf = 1000000009;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

int i, j, k, m, n, l, ans;
char ss[1000002];
int x[505], y[505], r[505], c[505];
int g[505][505];

int main() {
//	freopen("d.in", "r", stdin);

	freopen("D-small-attempt1.in", "r", stdin);
	freopen("D-small-attempt1.out", "w", stdout);

//	freopen("D-large.in", "r", stdin);
//	freopen("D-large.out", "w", stdout);

	int tt, tn; cin >> tn;
	F1(tt,tn) {
		cin >> n;
		F0(i,n) {
			cin >> x[i] >> y[i] >> r[i] >> c[i];
		}
		memset(g, 0, sizeof(g));
		F0(i,n) F0(j,n) if ((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]) <= r[i]*r[i]) g[i][j] = 1;

		ans = 0;

		F0(k,(1<<n)) {
			int cur = 0, ok = 1;
			F0(i,n) if ((1<<i)&k) {
				cur += c[i];
				F0(j,n) if ((1<<j)&k); else if (g[i][j]) break;
				if (j<n) break;
			}
			if (i==n) ans = max(ans, cur);
		}
		printf("Case #%d: ", tt);
	
		cout << ans << endl;
	}
	
	return 0;
}
