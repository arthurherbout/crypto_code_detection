#include <bits/stdc++.h>
#define lli long long int
#define s(x) scanf("%lld", &x);
lli mod = 1000000007;
using namespace std;

int main()
{
    freopen("C-small-attempt0.in", "r", stdin);
    freopen("out.txt", "w", stdout);

    lli i, t, ta, a[1005], ans, n, j, visit[1005], ma, l, k, s, e, tem, tem1;

    //string s, p;

    s(t);
    ta = 1;
    while(t--) {
        vector<lli>v;
        ans = 0;
        ma = INT_MIN;
        s(n);
        for (i = 1; i <= n; i++) {
            s(l);
            a[i] = l;
        }

        for (i = 0; i < 1005; i++) {
            visit[i] = 0;
        }

        visit[1] = 1;
        ans = 1;
        k = a[1];
        while(visit[k] != 1) {
            ans++;
            visit[k] = 1;
            tem = k;
            k = a[k];
        }

        for (i = 1; i <= n; i++) {
            if (a[i] == 1 || a[i] == tem) {
                if (visit[i] == 0) {
                    ans++;
                    break;
                }
            }
        }

        cout<<"Case #"<<ta<<": "<<ans<<endl;
        ta++;
    }
    return 0;
}
