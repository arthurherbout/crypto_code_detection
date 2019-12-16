#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ld = long double;
using D = double;
using uint = unsigned int;
template<typename T>
using pair2 = pair<T, T>;

#ifdef WIN32
    #define LLD "%I64d"
#else
    #define LLD "%lld"
#endif

#define pb push_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define fi first
#define se second

const int maxn = 1005;

vector<pair2<int>> from[maxn], to[maxn];
int go[2 * maxn];
int n;

inline int get(int a, int b)
{
    if (b >= a) return b - a;
    else return 24 - (a - b);
}

int main()
{
    int NT = 0;
    scanf("%d", &NT);
    for (int T = 1; T <= NT; T++)
    {
        printf("Case #%d:", T);
        
        scanf("%d", &n);
        for (int i = 0; i < n; i++)
        {
            from[i].clear();
            to[i].clear();
        }
        int answer = 0;
        for (int i = 0; i < 2 * n; i++)
        {
            int x = i / 2;
            int y, l, d;
            scanf("%d%d%d", &y, &l, &d);
            y--;
            answer += d;
            to[y].pb({(l + d) % 24, i});
            from[x].pb({l, i});
        }
        int ans = 1e9;
        for (int mask = 0; mask < (1 << n); mask++)
        {
            int curans = 0;
            for (int i = 1; i < n; i++)
            {
                if (mask & (1 << i))
                {
                    curans += get(to[i][0].fi, from[i][1].fi);
                    curans += get(to[i][1].fi, from[i][0].fi);
                    go[to[i][0].se] = from[i][1].se;
                    go[to[i][1].se] = from[i][0].se;
                } else
                {
                    curans += get(to[i][0].fi, from[i][0].fi);
                    curans += get(to[i][1].fi, from[i][1].fi);
                    go[to[i][0].se] = from[i][0].se;
                    go[to[i][1].se] = from[i][1].se;
                }
            }
            if (mask & 1)
            {
                curans += min(get(to[0][0].fi, from[0][1].fi) + get(0, from[0][0].fi), get(to[0][1].fi, from[0][0].fi) + get(0, from[0][1].fi));
                go[to[0][0].se] = from[0][1].se;
                go[to[0][1].se] = from[0][0].se;
            } else
            {
                curans += min(get(to[0][0].fi, from[0][0].fi) + get(0, from[0][1].fi), get(to[0][1].fi, from[0][1].fi) + get(0, from[0][0].fi));
                go[to[0][0].se] = from[0][0].se;
                go[to[0][1].se] = from[0][1].se;
            }
            int cur = 0;
            int cnt = 0;
            while (true)
            {
                cur = go[cur];
                cnt++;
                if (cur == 0) break;
            }
            if (cnt != 2 * n) continue;
            ans = min(ans, curans);
        }
        printf(" %d\n", answer + ans);

        fprintf(stderr, "%d/%d cases done!\n", T, NT);
    }
    return 0;
}
