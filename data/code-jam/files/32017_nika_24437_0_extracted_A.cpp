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

int i, j, k, m, n, l;

ll x[100001], y[100001];

int main() {
	freopen("A-small-attempt0.in", "r", stdin);
	freopen("A-small-attempt0.out", "w", stdout);

	int tt, tn; cin >> tn;
	F1(tt,tn) {


		ll A, B, C, D, x0, y0, M;
		cin >> n >> A >> B >> C >> D >> x0 >> y0 >> M;

		x[0] = x0;
		y[0] = y0;
		
		F1(i,n-1) {
			x[i] = (A * x[i-1] + B) % M;
			y[i] = (C * y[i-1] + D) % M;
		}
		ll ans = 0;
		F0(i,n) F0(j,n) if (j>i) F0(k,n) if (k>j)
			if ((x[i]+x[j]+x[k])%3 == 0 && (y[i]+y[j]+y[k])%3==0) ans++;
		printf("Case #%d: %d\n", tt, ans);
	}


	
	return 0;
}
