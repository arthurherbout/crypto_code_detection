#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <cstring>
#include <algorithm>
#include <queue>
#include <sstream>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
#define SZ(x) (int)(x.size())
#define F0(i,n) for(int i=0;i<n;i++)
#define F1(i,n) for(int i=1;i<=n;i++)
#define CL(a,x) memset(x, a, sizeof(x))
const int inf = 1000000009;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

int i, j, k, m, n, l, ans;
char ss[1000002];

int a[1<<15], b[1<<15];

int main() {
//    freopen("x.in", "r", stdin);

    freopen("C-small-attempt0.in", "r", stdin);
    freopen("C-small-attempt0.out", "w", stdout);

//	freopen("C-large.in", "r", stdin);
//	freopen("C-large.out", "w", stdout);

	int tt, tn; cin >> tn;
	F1(tt,tn) {
        cerr << tt << endl;
        cin >> n;


        map<int,int> M;
        m = 1<<15;
        F0(i,m) a[i] = 0;
        k = 0;
        string s;
        F0(i,n) {
            F0(j,m) b[j] = inf;

            cin >> s >> l;
            if (s == "E") {
                if (l == 0) {
                    F0(j,m) {
                        b[j] = min(b[j], a[j] + 1);
                        F0(o,15) if (((1<<o)&j)==0) {
                            b[j^(1<<o)] = min(b[j^(1<<o)], a[j]);
                        }
                    }
                } else {
                    if (!M.count(l)) M[l] = k++;
                    l = M[l];
                    F0(j,m) {
                        int o = l;
                        if (((1<<o)&j)==0) {
                            b[j^(1<<o)] = min(b[j^(1<<o)], a[j]);
                        }
                    }
                }
            }
            else {
                if (l == 0) {
                    F0(j,m) {
                        b[j] = min(b[j], max(a[j] - 1, 0));
                        F0(o,15) if (((1<<o)&j)>0) {
                            b[j^(1<<o)] = min(b[j^(1<<o)], a[j]);
                        }
                    }
                } else {
                    if (!M.count(l)) M[l] = k++;
                    l = M[l];
                    F0(j,m) {
                        int o = l;
                        if (((1<<o)&j)>0) {
                            b[j^(1<<o)] = min(b[j^(1<<o)], a[j]);
                        }
                    }
                }
            }
            F0(j,m) a[j] = b[j];
        }
        int ans = inf;
        F0(j,m) ans = min(ans, a[j]+bc(j));

		printf("Case #%d: ", tt);
        if (ans >= inf/2) cout << "CRIME TIME"; else cout << ans;
        cout << endl;
	}

	return 0;
}
