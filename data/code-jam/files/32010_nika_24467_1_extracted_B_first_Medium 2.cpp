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
pii a[1000];
int c[1000];
string s[1000];

int d[301][301][301];

map<string, int> M;

int rec(int i, int c1, int c2) {
//	cout << i << " " << c1 << " " << c2 << endl;
	if (a[i].first >= 10000) return 0;
	int& ret = d[i][c1][c2];
	if (ret != -1) return ret;
//	cout << i << " " << c1 << " " << c2 << endl;

	ret = inf;

	for (int j = i+1; j < n; j++)
		if (a[j].second <= a[i].first+1) {
			if (c[i] == c[j]) ret = min(ret, 1+rec(j, c1, c2));
			else if (c1 == 0) ret = min(ret, 1+rec(j, c[i], 0));
			else if (c1 == c[j]) ret = min(ret, 1+rec(j, c[i], c2));
			else if (c2 == 0) ret = min(ret, 1+rec(j, c[i], c1));
			else if (c2 == c[j]) ret = min(ret, 1+rec(j, c[i], c1));
		}

	return ret;
}

// The second problem - should be harder than the first
int main() {

//	freopen("x.in", "r", stdin);

//	freopen("B-small-attempt0.in", "r", stdin);
//	freopen("B-small-attempt02.out", "w", stdout);

	freopen("B-large.in", "r", stdin);
	freopen("B-large.out", "w", stdout);

	int tt, tn; cin >> tn;

	F1(tt,tn) {
		printf("Case #%d: ", tt);

		cin >> n;

		F0(i,n) cin >> s[i] >> a[i].second >> a[i].first;

		M.clear();

		int cnt = 0;
		F0(i,n) {
			if (M[s[i]] == 0) M[s[i]] = ++cnt;
			c[i] = M[s[i]];
		}

		int ans = inf;

		memset(d, -1, sizeof(d));

		F0(i,n) for (j = i+1; j < n; j++)
			if (a[i].second > a[j].second) {
				swap(a[i], a[j]);
				swap(c[i], c[j]);
			}

		F0(i,n) if (a[i].second <= 1) ans = min(ans, 1 + rec(i, 0, 0) );

		if (ans > 10000) cout << "IMPOSSIBLE"; else cout << ans;
		printf("\n");
	}
	
	return 0;
}
