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
double ans;
pii p[1000002];
#define DIST(i,j) (hypot(1.0*(p[i].first-p[j].first),1.0*(p[i].second-p[j].second)))
typedef pair<double, int> point;
char ss[1000002];


int main() {
//	freopen("b.in", "r", stdin);

	freopen("b1.in", "r", stdin);
	freopen("b1.out", "w", stdout);

//	freopen("B-large.in", "r", stdin);
//	freopen("B-large.out", "w", stdout);

	int tt, tn; cin >> tn;
	F1(tt,tn) {

		printf("Case #%d: ", tt);

		cin >> n;
		F0(i,n) {
			scanf("%d%d", &p[i].first, &p[i].second);
		}
		ans = 1e100;
		sort(p, p+n);
		F0(i,n) {
			int L = min(i+20, n);
			for (j = i+1; j < L; j++)
				for (k = j+1; k < L; k++) {
					double u = DIST(i,j)+DIST(j,k)+DIST(i,k);
					if (u < ans) ans = u;
				}
		}
		F0(i,n) {
			swap(p[i].first, p[i].second);
		}
		sort(p, p+n);
		F0(i,n) {
			int L = min(i+20, n);
			for (j = i+1; j < L; j++)
				for (k = j+1; k < L; k++) {
					double u = DIST(i,j)+DIST(j,k)+DIST(i,k);
					if (u < ans) ans = u;
				}
		}

		printf("%.10lf\n", ans);
	}
	
	return 0;
}
