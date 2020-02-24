#include <cstdio>
#include <cstdlib>
#include <cstring>
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
typedef pair<ll,ll> pii;
#define SZ(x) (int)(x.size())
#define F0(i,n) for(i=0;i<n;i++)
#define F1(i,n) for(i=1;i<=n;i++)
#define CL(x,a) memset(a,x,sizeof(a));
const int inf = 1000000009;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

ll i, j, k, m, n, l;
char ss[1000002];
pii p[201];
ll ans, F, M;
vector<ll> pr, cnt;

ll get(ll x)
{
    ll ret = 0, sum = 0, total = 0;

    if (x < 0) throw 1;
    if (x <= F) return 0;
    x -= F;

    for (int i = 0; i < SZ(cnt); i++)
    {
        ret = max(ret, (x - sum) / pr[i] + total);
        if (cnt[i] > x / pr[i]) return ret;
        sum += cnt[i] * pr[i];
        total += cnt[i];
        if (sum <= x) ret = total;
        else return ret;
    }
    return ret;
}

int main() {
//    freopen("c.in", "r", stdin);

    freopen("C-small-attempt3.in", "r", stdin);
    freopen("C-small-attempt3.txt", "w", stdout);

//	  freopen("C-large.in", "r", stdin);
//	  freopen("C-large.txt", "w", stdout);

	int tt, tn; cin >> tn;
	F1(tt,tn) {
        cin >> M >> F >> n;
        F0(i,n) { cin >> p[i].first >> p[i].second; p[i].second++; }
        sort(p,p+n);
        pr.clear();
        cnt.clear();
        ll last = 0;

        F0(i,n) if (p[i].second > last)
        {
            pr.push_back(p[i].first);
            cnt.push_back(p[i].second - last);
            last = p[i].second;
        }

        n = SZ(pr);
        ll sum = 0, total = 0;

        ans = get(M);
        for (int i = 0; i < SZ(cnt); i++)
        {
            if (cnt[i] > (M-sum-F)/pr[i]) break;
            sum += cnt[i] * pr[i];
            total += cnt[i];

            if (sum + F > M) break;
            ll op = M / (sum + F);
            ans = max(ans, op * total + get(M - op * (sum + F)));

            if (op > 0)
            {
                op--;
                ans = max(ans, op * total + get(M - op * (sum + F)));
            }

        }

        printf("Case #%d: ", tt);

        cout << ans << endl;
	}
	
	return 0;
}
