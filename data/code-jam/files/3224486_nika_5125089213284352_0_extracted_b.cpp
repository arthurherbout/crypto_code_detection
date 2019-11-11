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

int i, j, k, m, n, l, cnt[5];
int p[105];
vector<int> adj[105];
string s[5], q;

vector<int> merge(const vector<int>& a, const vector<int>& b) {
	vector<int> ret;
	int na = SZ(a), nb = SZ(b), i = 0, j = 0;
	while (na > 0 || nb > 0) {
		int z = rand() % (na + nb);
		if (z < na) {
			ret.push_back(a[i++]); na--;
		}
		else {
			ret.push_back(b[j++]); nb--;
		}
	}
	return ret;
}

vector<int> gen(int i) {
	vector<int> ret;
	for (int j : adj[i]) {
		vector<int> w = gen(j);
		ret = merge(ret, w);
	}
	ret.insert(ret.begin(), i);
	return ret;
}

int main() {
    //freopen("x.in", "r", stdin);

	freopen("B-small-attempt0.in", "r", stdin);
	freopen("B-small-attempt0.out", "w", stdout);

	//freopen("B-large.in", "r", stdin);
	//freopen("B-large.out", "w", stdout);

	int tt, tn; cin >> tn;

	F1(tt,tn) {
		cerr << tt << endl;
		cin >> n;
		F0(i, n + 1) adj[i].clear();
		F1(i, n) {
			cin >> p[i];
			adj[p[i]].push_back(i);
		}
		cin >> q;
		cin >> m;
		F0(i, m) cin >> s[i];

		int iters = 10000;
		F0(i, m) cnt[i] = 0;
		F0(iter, iters) {
			vector<int> v = gen(0);
			string z = "";
			F1(i, SZ(v) - 1) z += q[v[i] - 1];
			F0(i, m) if (z.find(s[i]) != -1) cnt[i]++;
		}

		printf("Case #%d:", tt);
		F0(i, m) printf(" %.6lf", cnt[i] * 1.0 / iters);
		printf("\n");
	}
	return 0;
}
