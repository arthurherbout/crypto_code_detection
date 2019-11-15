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
const double inf = 1e100;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

int i, j, k, m, n, l, lane[55], v[55], sx[55];
double ans, x[55];
char ss[1000002];

int main() {
//    freopen("c.in", "r", stdin);

    freopen("C-small-attempt2.in", "r", stdin);
    freopen("C-small-attempt2.out", "w", stdout);

//	freopen("C-large.in", "r", stdin);
//	freopen("C-large.out", "w", stdout);

	int tt, tn; cin >> tn;
	F1(tt,tn) {
        cin >> n;
        F0(i,n) {
            char c;
            cin >> c >> v[i] >> sx[i];
            lane[i] = (c == 'R');
        }

        ans = inf;
        F0(i,n) F0(j,n) if (v[i] > v[j] && sx[i] + 5.0 < sx[j] - eps)
        {
            double t = (sx[j]-sx[i]-5.0) / (v[i] - v[j]);
            F0(k,n) x[k] = sx[k] + v[k] * t;
            F0(k,n) if (k != i && k != j && x[k] >= x[i]-eps && x[k] <= x[j]+eps)
            {
                ans = min(ans, t);
            }

            F0(k,n) if (k != i && k != j && x[k] <= x[i] && x[k] >= x[i] - 5.0 && v[k] < v[i])
                    F0(l,n) if (l != i && l != j && x[l] >= x[j] && x[l] <= x[j] + 5.0 && v[l] < v[j])
            {
                ans = min(ans, t);
            }
        }

        printf("Case #%d: ", tt);
        if (ans == inf)
        {
            cout << "Possible" << endl;
        }
        else
        {
            printf("%.10lf\n", ans);
        }
	}
	
	return 0;
}
