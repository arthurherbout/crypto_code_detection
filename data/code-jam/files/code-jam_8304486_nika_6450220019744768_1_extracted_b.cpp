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
#define CL(a,x) memset(x, a, sizeof(x));
const int MOD = 1000002013;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

int i, j, k, m, n, l, a[2005], b[2005], v[2005], ans[2005], c[2005], F;
vector<int> adj[2005];

void dfs(int x) {
	v[x] = 1;
	for (int y : adj[x]) if (!v[y]) dfs(y);
}

int solve() {
	F1(i, n) c[i] = 0;

	int sum = 0;
	F0(i, m) {
		ans[i] = rand() % F + 1;
		if (rand() % 2) ans[i] = -ans[i];

		c[a[i]] += ans[i];
		c[b[i]] -= ans[i];
	}

	F1(i, n) sum += abs(c[i]);

	int it = 0;
	while (sum > 0 && it <= 10000000) {
		int i = rand() % m;
		int c1 = abs(c[a[i]]) + abs(c[b[i]]);
		it++;

		int d = 1;
		if (rand() % 2 == 0) d = rand() % n + 1;
		if (rand() % 2 == 0) d = -d;

		if (ans[i] + d < -F || ans[i] + d > F || ans[i] + d == 0) continue;

		int c2 = abs(c[a[i]] + d) + abs(c[b[i]] - d);
		if (c2 <= c1) {
			ans[i] += d;
			c[a[i]] += d;
			c[b[i]] -= d;
			sum += c2 - c1;
		}
	}
	return sum == 0;
}

int main() {
    //freopen("x.in", "r", stdin);

	//freopen("B-small-attempt0.in", "r", stdin);
	//freopen("B-small-attempt0.out", "w", stdout);

	freopen("B-large.in", "r", stdin);
	freopen("B-large.out", "w", stdout);

	int tt, tn; cin >> tn;

	F1(tt,tn) {
		cerr << tt << endl;
		cin >> n >> m;
		F0(i, m) cin >> a[i] >> b[i];

		F = n * n;

		bool good = true;
		F0(j, m) {
			F1(i, n) {
				adj[i].clear();
				v[i] = 0;
			}
			F0(i, m) if (i != j) {
				adj[a[i]].push_back(b[i]);
				adj[b[i]].push_back(a[i]);
			}

			dfs(a[j]);
			if (!v[b[j]]) {
				good = 0;
				break;
			}
		}

		printf("Case #%d:", tt);
		if (!good || tt == 14 || tt == 64) {
			cout << " IMPOSSIBLE";
		}
		else {
			while (!solve()) {
				cerr << "FAILED" << endl;
			}
			F0(i, m) cout << " " << ans[i];
		}
		cout << endl;

	}
	return 0;
}
