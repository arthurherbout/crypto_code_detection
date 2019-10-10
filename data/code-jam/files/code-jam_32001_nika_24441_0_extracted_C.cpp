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
ll x[10000], y[10000], z[10000], p[10000];

int ok(const vector<double> V, double Q) {
	double v1 = -1e100, v2 = 1e100;
	for (int i = 0; i < SZ(V); i++) {
		v1 = max(v1, V[i] - p[i] * Q);
		v2 = min(v2, V[i] + p[i] * Q);
	}
	return v1 <= v2;
}

int main() {

//	freopen("x.in", "r", stdin);

	freopen("C-small-attempt0.in", "r", stdin);
	freopen("C-small-attempt0.out", "w", stdout);

//	freopen("C-large.in", "r", stdin);
//	freopen("C-large.out", "w", stdout);

	int tt, tn; cin >> tn;
	F1(tt,tn) {
		printf("Case #%d: ", tt);
		vector<double> v0, v1, v2, v3;

		cin >> n;
		F0(i,n) {
			cin >> x[i] >> y[i] >> z[i] >> p[i];
			v0.push_back(x[i] + y[i] + z[i]);
			v1.push_back(x[i] - y[i] + z[i]);
			v2.push_back(x[i] + y[i] - z[i]);
			v3.push_back(x[i] - y[i] - z[i]);
		}

		double lo, mid, hi;
		lo = 0.0; hi = 1000000000.0;
		int it;
		F0(it,100) {
			mid = (lo + hi) / 2.0;
			if (ok(v0, mid) && ok(v1, mid) && ok(v2, mid) && ok(v3, mid)) hi = mid; else lo = mid;
		}
		printf("%.6lf", mid);
		printf("\n");
	}
	
	return 0;
}
