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

int i, j, k, m, n, l;
int a[1000], b[1000], c[1000];
string s[1000];

int d[10001];

map<string, int> M;

// The second problem - should be harder than the first
int main() {

//	freopen("x.in", "r", stdin);

	freopen("B-small-attempt0.in", "r", stdin);
	freopen("B-small-attempt0.out", "w", stdout);

//	freopen("B-large.in", "r", stdin);
//	freopen("B-large.out", "w", stdout);

	int tt, tn; cin >> tn;

	F1(tt,tn) {
		printf("Case #%d: ", tt);

		cin >> n;

		F0(i,n) cin >> s[i] >> a[i] >> b[i];

		M.clear();

		int cnt = 0;
		F0(i,n) {
			if (M[s[i]] == 0) M[s[i]] = ++cnt;
			c[i] = M[s[i]];
		}

		int ans = inf;

		F0(i,(1<<n)) {
			F1(j,10000) d[j] = 0;
			int q = 0;
			F0(j,n) if ((1<<j)&i) {
				q |= (1<<c[j]);
				for (k = a[j]; k <= b[j]; k++)
					d[k] = 1;
			}
			F1(j,10000) if (bc(d[j]) <= 0) break;
			
			if (j > 10000 && bc(q) <= 3) {
				ans = min(ans, bc(i));
			}

		}

		if (ans > 10000) cout << "IMPOSSIBLE"; else cout << ans;
		printf("\n");
	}
	
	return 0;
}
