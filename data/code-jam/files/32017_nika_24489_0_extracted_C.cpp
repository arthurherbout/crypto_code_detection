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
int a[100001], q[10001];

int main() {
	freopen("C-small-attempt0.in", "r", stdin);
	freopen("C-small-attempt0.out", "w", stdout);
	int tt, tn;
	cin >> tn;

	F1(tt,tn) {
		cin >> n >> k; F1(i,k) cin >> q[i];
		printf("Case #%d:", tt);
		F0(i,n) a[i] = -1;
		j = -1;
		F1(i,n) {
			l = i;
			while (l > (n-i+1)) l -= (n-i+1);
			while (l--) {
				while (1) {
					j = (j+1)%n;
					if (a[j] == -1) break;
				}
			}
			a[j] = i;
		}
		F1(i,k) printf(" %d", a[q[i]-1]);
		printf("\n");
	}

	return 0;
}
