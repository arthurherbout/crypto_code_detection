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

int i, j, k, m, n, l, p[100001];

ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }

void unite(int i, int j) {
	while (p[i] != -1) i = p[i];
	while (p[j] != -1) j = p[j];
	if (i != j) p[i] = j;
}

ll bigpr(ll n) {
	ll ret = 0;
	for (ll i = 2; i * i <= n; i++)
		if (n % i == 0) {
			ret = max(ret, i);
			while (n%i == 0) n /= i;
		}
	ret = max(ret, n);
	return ret;
}

int main() {
	freopen("B-small-attempt0.in", "r", stdin);
	freopen("B-small-attempt0.out", "w", stdout);

	int tn, tt;
	cin >> tn;
	F1(tt,tn) {
		ll A, B, P;
		cin >> A >> B >> P;
		for (i = A; i <= B; i++) p[i] = -1;
		for (i = A; i <= B; i++)
			for (j = A; j <= B; j++)
				if (i != j && bigpr(gcd(i, j)) >= P) unite(i, j);
		ll ans = 0;
		for (i = A; i <= B; i++)
			if (p[i] == -1) ans++;
		printf("Case #%d: %d\n", tt, ans);
	}

	return 0;
}
