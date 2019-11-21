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

int i, j, k, m, n, l, sn;
int ans, start[2005], duration[2005], e[2005], used[2005];

void go(int at, int cnt, int time) {
	if (cnt == 2 * n) {
		ans = min(ans, time);
		return;
	}

	for (int x = 0; x < 2; x++) {
		int j = 2 * at + x;
		if (used[j]) continue;

		used[j] = 1;
		int newt = time;

		while (newt % 24 != start[j]) newt++;
		newt += duration[j];
		go(e[j], cnt + 1, newt);
		used[j] = 0;
	}
}

int main() {
    //freopen("x.in", "r", stdin);

	freopen("C-small-attempt0.in", "r", stdin);
	freopen("C-small-attempt0.out", "w", stdout);

	//freopen("C-large.in", "r", stdin);
	//freopen("C-large.out", "w", stdout);

	int tt, tn; cin >> tn;

	F1(tt,tn) {
		//cerr << tt << endl;
		cin >> n;
		F0(i, 2 * n) {
			cin >> e[i] >> start[i] >> duration[i];
			e[i]--;
		}

		ans = 1e9;
		
		F0(i, 2 * n) used[i] = 0;
		go(0, 0, 0);

		printf("Case #%d: %d\n", tt, ans);
	}
	return 0;
}
