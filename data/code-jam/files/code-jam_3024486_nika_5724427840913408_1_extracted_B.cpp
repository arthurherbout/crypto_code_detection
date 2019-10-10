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
#include <cstring>
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
int G[1002], H[1002], d[2][101][201][2005], P, Q;
char ss[1000002];

int go(int tower, int at, int hp, int saved)
{
    if (hp < 0) hp = 0;
    if (saved >= 1000) saved = 1000;

    if (at == n) return 0;
    int& ret = d[tower][at][hp][saved];
    if (ret != -1) return ret;
    ret = 0;

    if (tower)
    {
        if (hp == 0)
        {
            int hp2 = H[at + 1], add = 0;
            for (int i = 0; i <= saved; i++) {
                ret = max(ret, add + go(1, at+1, hp2, saved - i));
                if (hp2 <= 0) break;
                else {
                    hp2 -= P;
                    if (hp2 <= 0) add = G[at + 1];
                }
            }
        }
        else
        {
            ret = go(0, at, hp-Q, saved);
        }
        return ret;
    }


    if (hp == 0) {
        int hp2 = H[at + 1], add = 0;
        for (int i = 0; i <= saved; i++) {
            ret = max(ret, add + go(0, at+1, hp2, saved - i));
            if (hp2 <= 0) break;
            else {
                hp2 -= P;
                if (hp2 <= 0) add = G[at + 1];
            }
        }
    } else {
        ret = go(1, at, hp, saved + 1);
        int add = 0;
        if (hp <= P) add = G[at];
        ret = max(ret, add + go(1, at, hp-P, saved));
    }

    return ret;
}

int rec(int at)
{
    int res = 0;
    if (at == n) return 0;
    if (H[at] <= 0) return rec(at+1);
    for (int i = at; i <= n; i++) if (H[i] > 0) {
        int cnt = 0;
        H[i] -= P; if (H[i] <= 0) cnt += G[i];

        int j;
        for (j = 0; j < n; j++) if (H[j] > 0) break;
        if (j < n) H[j] -= Q;

        cnt += rec(at);

        if (j < n) H[j] += Q;
        H[i] += P;

        res = max(cnt, res);
    }
    return res;
}

int main() {
 //   freopen("x.in", "r", stdin);

    //  freopen("B-small-attempt2.in", "r", stdin);
    // freopen("B-small-attempt2.out", "w", stdout);

    freopen("B-large.in", "r", stdin);
    freopen("B-large.out", "w", stdout);

	int tt, tn; cin >> tn;
	F1(tt,tn) {
        cerr << tt << endl;
        cin >> P >> Q >> n;
        CL(0,H);
        CL(0,G);
        F0(i,n) cin >> H[i] >> G[i];
        CL(-1, d);

        ans = go(0, 0, H[0], 0);

/*
        if (n > 1) continue;
        H[n] = inf;
        int ans2 = rec(0);
        cerr << ans2 << endl;
        if (ans != ans2) {
            cout << P << " " << Q << endl;
            cout << n << endl;
            F0(i,n) cout << H[i] << " " << G[i] << endl;
            cerr << ans << " " << ans2 << endl;
            exit(0);
        }
*/
        printf("Case #%d: %d\n", tt, ans);
	}

	return 0;
}
