#include <bits/stdc++.h>
#define inf 0x3f3f3f3f
#define Inf 0x3FFFFFFFFFFFFFFFLL
#define rep(i, n) for (int i = 0; i < (n); ++i)
#define Rep(i, n) for (int i = 1; i <= (n); ++i)
#define FOR(i, c) for(__typeof((c).begin()) i = (c).begin(); i != (c).end(); ++i)
#define clr(x, a) memset(x, (a), sizeof x)
#define RD(x) scanf("%d", &x)
#define PB push_back
#define MP make_pair
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
int const N = 2020;
ll const mod = 1000000007LL;
double x[N], y[N], z[N];
double vx[N], vy[N], vz[N];
int n;
int parents[N];
int Find(int a) {
	return parents[a] < 0 ? a : parents[a] = Find(parents[a]);
}
void Union(int a,int b) {
	a = Find(a), b = Find(b);
	if (parents[a] < parents[b]) parents[a] += parents[b], parents[b] = a;
	else parents[b] += parents[a], parents[a] = b;
}
void init() { clr(parents, 0xff); }
double s;
double dis(int a, int b) {
	return (x[a] - x[b]) * (x[a] - x[b]) + (y[a] - y[b]) * (y[a] - y[b]) + (z[a] - z[b]) * (z[a] - z[b]);
}
bool check(double mid) {
	init();
	double mt = mid * mid;
	rep(i, n) {
		for (int j = i + 1; j < n; ++j) {
			if (Find(i) != Find(j) && dis(i, j) + 1e-8 < mt) {
				Union(i, j);
			}
		}
	}
	return Find(0) == Find(1);
}

int main() {
	freopen("C-small-attempt0.in", "r", stdin);
	freopen("C-small-ans", "w", stdout);
	int T, ca = 1; scanf("%d", &T);
	while (T--) {
		scanf("%d %lf", &n, &s);
		rep(i, n) {
			scanf("%lf%lf%lf%lf%lf%lf", &x[i], &y[i], &z[i], &vx[i], &vy[i], &vz[i]);
		}
		double low = 0, high = 1e12, mid;
		rep(i, 100) {
			mid = (low + high) * 0.5;
			if (check(mid)) {
				high = mid;
			} else low = mid;
		}

		printf("Case #%d: %.10lf\n", ca++, low);
	}
	return 0;
}


