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
const int MOD = 1000002013;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

const int N = 1001;
int i, j, k, m, n, l;
int S;
int x[N], y[N], z[N];
int vx[N], vy[N], vz[N], v[N];
double P, Q, R;

void dfs(int i) {
	if (v[1]) return;
	if (v[i]) return;
	v[i] = 1;
	F0(j, n) {
		double d = hypot(1.0*x[i] - x[j], 1.0*y[i] - y[j]);
		d = hypot(d, 1.0*z[i] - z[j]);
		if (d <= R) dfs(j);
	}
}

int ok() {
	F0(i, n) v[i] = 0;
	dfs(0);
	return v[1];
}

int main() {
    //freopen("x.in", "r", stdin);

	freopen("C-small-attempt0.in", "r", stdin);
	freopen("C-small-attempt0.out", "w", stdout);

	//freopen("C-large.in", "r", stdin);
	//freopen("C-large.out", "w", stdout);

	int tt, tn; cin >> tn;

	F1(tt,tn) {
		cerr << tt << endl;
		cin >> n >> S;
		F0(i, n) {
			cin >> x[i] >> y[i] >> z[i];
			cin >> vx[i] >> vy[i] >> vz[i];
		}

		P = 0.0; Q = 10000.0;
		F0(it, 50) {
			R = (P + Q) / 2;
			if (ok()) Q = R; else P = R;
		}

		printf("Case #%d: %.6lf\n", tt, P);
	}
	return 0;
}
