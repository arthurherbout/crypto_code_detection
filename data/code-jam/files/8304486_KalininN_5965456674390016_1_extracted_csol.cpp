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

const int maxn = 2005;

vector<pair2<int>> from[maxn], to[maxn];
int go[2 * maxn];
int comp[2 * maxn];
vector<pair<int, pair2<int>>> edges;
int cost[maxn][2];
int curans;
int p[maxn];
bool was[maxn];
int n;

inline int get(int a, int b)
{
    if (b >= a) return b - a;
    else return 24 - (a - b);
}

int find(int x)
{
    return p[x] == x ? p[x] : p[x] = find(p[x]);
}

void unite(int a, int b)
{
    p[b] = a;
}

void solve()
{
    for (int i = 1; i < n; i++)
    {
        if (get(to[i][0].fi, from[i][0].fi) + get(to[i][1].fi, from[i][1].fi) > get(to[i][0].fi, from[i][1].fi) + get(to[i][1].fi, from[i][0].fi))
        {
            swap(from[i][0], from[i][1]);
        }
        cost[i][0] = get(to[i][0].fi, from[i][0].fi) + get(to[i][1].fi, from[i][1].fi);
        cost[i][1] = get(to[i][0].fi, from[i][1].fi) + get(to[i][1].fi, from[i][0].fi);
        assert(cost[i][0] <= cost[i][1]);
        curans += cost[i][0];
        go[to[i][0].se] = from[i][0].se;
        go[to[i][1].se] = from[i][1].se;
//         cout << cost[i][0] << ' ' << cost[i][1] << endl;
    }
    for (int i = 0; i < 2 * n; i++) was[i] = false;
    int ncomp = 0;
    for (int i = 0; i < 2 * n; i++) if (!was[i])
    {
        int cur = i;
        while (true)
        {
            comp[cur] = ncomp;
            cur = go[cur];
            was[cur] = true;
            if (cur == i) break;
        }
        ncomp++;
    }
    edges.clear();
    for (int i = 0; i < ncomp; i++)
    {
        p[i] = i;
    }
    for (int i = 1; i < n; i++) if (comp[to[i][0].se] != comp[to[i][1].se])
    {
        edges.pb({cost[i][1] - cost[i][0], {comp[to[i][0].se], comp[to[i][1].se]}});
    }
    sort(all(edges));
    for (auto t : edges)
    {
        int a = find(t.se.fi);
        int b = find(t.se.se);
        if (a == b) continue;
        curans += t.fi;
        unite(a, b);
    }
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
        {
            curans = min(get(to[0][0].fi, from[0][1].fi) + get(0, from[0][0].fi), get(to[0][1].fi, from[0][0].fi) + get(0, from[0][1].fi));
            go[to[0][0].se] = from[0][1].se;
            go[to[0][1].se] = from[0][0].se;
//             cout << "try " << curans << endl;
            solve();
//             cout << "got " << curans << endl;
            ans = min(ans, curans);
        }
        {
            curans = min(get(to[0][0].fi, from[0][0].fi) + get(0, from[0][1].fi), get(to[0][1].fi, from[0][1].fi) + get(0, from[0][0].fi));
            go[to[0][0].se] = from[0][0].se;
            go[to[0][1].se] = from[0][1].se;
//             cout << "try " << curans << endl;
            solve();
//             cout << "got " << curans << endl;
            ans = min(ans, curans);
        }
        printf(" %d\n", answer + ans);

        fprintf(stderr, "%d/%d cases done!\n", T, NT);
    }
    return 0;
}
