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

struct tedge
{
    int to, id, t;
};

vector<tedge> gr[maxn];
int wait[maxn];
int was[maxn];
int n, m;
int answer[maxn * 2];
bool ok;
int lvl[maxn];

void go(int cur, int prid = -1, int curlvl = 0)
{
//     cout << "go " << cur << endl;
    lvl[cur] = curlvl;
    was[cur] = 1;
    for (auto t : gr[cur]) if (t.id != prid)
    {
        int v = t.to;
        if (was[v] == 1)
        {
            wait[lvl[v]]++;
            answer[t.id] = t.t;
//             cout << cur << ' ' << v << " easy " << t.t << endl;
            continue;
        }
        if (was[v] == 2) continue;
        int sum = 0;
        for (int i = 0; i <= curlvl; i++) sum += wait[i];
        go(v, t.id, curlvl + 1);
        int newsum = 0;
        for (int i = 0; i <= curlvl; i++) newsum += wait[i];
        int add = newsum - sum;
        sum = newsum;
        if (add == 0) ok = false;
        answer[t.id] = t.t * add;
    }
    was[cur] = 2;
//     cout << "end " << cur << endl;
}

int main()
{
    int NT = 0;
    scanf("%d", &NT);
    for (int T = 1; T <= NT; T++)
    {
        printf("Case #%d:", T);
        
        scanf("%d%d", &n, &m);
        for (int i = 0; i < n; i++)
        {
            was[i] = 0;
            gr[i].clear();
        }
        for (int i = 0; i < m; i++)
        {
            int a, b;
            scanf("%d%d", &a, &b);
            a--, b--;
            gr[a].pb({b, i, 1});
            gr[b].pb({a, i, -1});
        }
        ok = true;
        for (int i = 0; i < n; i++) if (!was[i]) go(i);
        if (!ok)
        {
            printf(" IMPOSSIBLE\n");
            continue;
        }
        for (int i = 0; i < m; i++) printf(" %d", answer[i]);
        printf("\n");

        fprintf(stderr, "%d/%d cases done!\n", T, NT);
    }
    return 0;
}
