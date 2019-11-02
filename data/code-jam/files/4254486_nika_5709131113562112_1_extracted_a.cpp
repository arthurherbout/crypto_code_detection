#pragma comment(linker, "/STACK:100000000")
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
int dx[] = { 0, -1, 0, 1 };
int dy[] = { 1, 0, -1, 0 };
int i, j, k, n, l;
int ans, cnt;
bool keep[1000005], inside[1000005];
ll D, s[1000005], m[1000005];
vector<int> adj[1000005];

void dfs1(int i) {
	keep[i] = 0; cnt--;
	for (int j : adj[i]) if (keep[j]) dfs1(j);
}

void dfs2(int i) {
	keep[i] = 1; cnt++;
	for (int j : adj[i]) if (inside[j]) dfs2(j);
}

int main() {
    //freopen("x.in", "r", stdin);

	//freopen("A-small-attempt0.in", "r", stdin);
	//freopen("A-small-attempt0.out", "w", stdout);

	freopen("A-large.in", "r", stdin);
	freopen("A-large.out", "w", stdout);

	int tt, tn; cin >> tn;

	F1(tt,tn) {
		cerr << tt << endl;
		ll AS, CS, RS, AM, CM, RM;
		cin >> n >> D;
		cin >> s[0] >> AS >> CS >> RS;
		cin >> m[0] >> AM >> CM >> RM;

		F0(i, n - 1) {
			s[i + 1] = (s[i] * AS + CS) % RS;
			m[i + 1] = (m[i] * AM + CM) % RM;
		}
		F0(i, n) adj[i].clear();
		F1(i, n - 1) {
			m[i] = m[i] % i;
			adj[m[i]].push_back(i);
		}

		vector<pair<pii, int> > events;
		F0(i, n) {
			events.push_back(make_pair(pii(s[i] - D, 1), i));
			events.push_back(make_pair(pii(s[i] + 1, -1), i));
		}
		sort(events.begin(), events.end());
		F0(i, n) keep[i] = inside[i] = false; cnt = 0;

		ans = 0;
		int iter = 0;
		for (auto x : events) {
			int i = x.second;

			if (x.first.second == -1) {
				//cout << "del " << i << endl;
				inside[i] = false;
				if (keep[i]) {
					dfs1(i);
				}
			}
			else {
				//cout << "add " << i << endl;
				inside[i] = true;
				if (keep[m[i]] || i == 0) {
					dfs2(i);
				}
			}

			ans = max(ans, cnt);
		}

		printf("Case #%d: ", tt);
		cout << ans << endl;
	}
	return 0;
}
