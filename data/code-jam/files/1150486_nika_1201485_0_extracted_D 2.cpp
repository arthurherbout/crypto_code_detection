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

int i, j, k, m, n, l, ans;
int a[401][401], d[401], good[401];
vector<int> adj[401];

vector<int> bfs(int start) {
	vector<int> ret(n, -1);
	ret[start] = 0;
	queue<int> Q;
	Q.push(start);
	while (!Q.empty()) {
		int i = Q.front();
		Q.pop();
		int sz = SZ(adj[i]), j;
		F0(j,sz) {
			int k = adj[i][j];
			if (ret[k] == -1) {
				ret[k] = ret[i] + 1;
				Q.push(k);
			}
		}
	}
	return ret;
}

int main() {
//	freopen("d.in", "r", stdin);

	freopen("D-small-attempt2.in", "r", stdin);
	freopen("D-small-attempt2.out", "w", stdout);

//	freopen("D-large.in", "r", stdin);
//	freopen("D-large.out", "w", stdout);

	int tt, tn; cin >> tn;
	F1(tt,tn) {
		printf("Case #%d: ", tt);

		cin >> n >> m;
		memset(a, 0, sizeof(a));
		F0(i,n) adj[i].clear();
		while (m--) {
			scanf("%d,%d", &i, &j);
			adj[i].push_back(j);
			adj[j].push_back(i);
			a[i][j] = a[j][i] = 1;
		}

		vector<int> q1 = bfs(0), q2 = bfs(1);
		k = q1[1];
		cout << k-1 << " ";
		vector<pii> V;

		F0(i,n) {
			d[i] = -1;
			good[i] = ((q1[i] + q2[i]) == k);
			if (good[i])
				V.push_back( pii(q1[i], i) );
		}
		sort(V.begin(), V.end());

		int z;
		F0(z,SZ(V)) {
			i = V[z].second;
			d[i] = 0;
			if (i == 0) {
				F0(j,n) if (a[i][j] && !good[j]) d[i]++;
			} else if (i == 1) {
				ans = 0;
				F0(j,n) if (a[i][j] && d[j] != -1 && q1[j] + 1 == q1[i]) {
					int curr = d[j] + 1;
					ans = max(curr, ans);
				}
			} else {
				d[i] = 0;
				F0(j,n) if (a[i][j] && d[j] != -1 && q1[j] + 1 == q1[i]) {
					int curr = d[j];
					F0(k,n) {
						if ((!good[k] || q1[k] != q1[i]+1) && a[i][k] && !a[j][k] && k != j) curr++;
						if (good[k] && q1[k] == q1[i] && a[j][k] && k != i) curr++;
					}
					d[i] = max(curr, d[i]);
					if (d[i] > n) {
						i = i;
					}
				}
			}
		}

		cout << ans << endl;
	}
	
	return 0;
}
