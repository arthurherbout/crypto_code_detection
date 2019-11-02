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

int i, j, k, m, n, l;
string good[101], bad;

int main() {
    //freopen("x.in", "r", stdin);

	freopen("D-small-attempt0.in", "r", stdin);
	freopen("D-small-attempt0.out", "w", stdout);

	//freopen("D-large.in", "r", stdin);
	//freopen("D-large.out", "w", stdout);

	int tt, tn; cin >> tn;

	F1(tt,tn) {
		//cerr << tt << endl;
		cin >> n >> l;
		F0(i, n) cin >> good[i];
		cin >> bad;

		printf("Case #%d: ", tt);
		bool possible = true;
		F0(i, n) if (good[i] == bad) possible = false;
		if (!possible) {
			cout << "IMPOSSIBLE" << endl;
		}
		else {
			if (l == 1) cout << "? 0"; else {
				cout << string(l - 1, '1') << " ";
				F0(i, l) cout << "0?";
			}
			cout << endl;
		}
	}
	return 0;
}
