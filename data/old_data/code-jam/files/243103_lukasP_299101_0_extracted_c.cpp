#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>
#include <queue>
#include <bitset>
#include <utility>
#include <list>
#include <numeric>

#include <cstdio>
#include <cmath>
#include <cctype>
using namespace std;

#define REP(i,n) for(__typeof(n) i=0; i<(n); ++i)
#define FOR(i,a,b) for(__typeof(b) i=a; i<(b); ++i)
#define FOREACH(it,c) for(__typeof((c).begin()) it=(c).begin(); it!=(c).end(); ++it)

typedef long long ll;
typedef pair<int, int> PI;
template <class T> void zlepsi(T &a, T b)
{
    a = max(a, b);
}
int main() {
    int tt; scanf("%d", &tt);
    REP(sd,tt)
    {
        int n; scanf("%d", &n);
        vector<PI> a(n);
        REP(i,n) scanf("%d %d", &a[i].first, &a[i].second);
        REP(i,n) a[i].second--;
        vector<int> q(30, -1);
        sort(a.begin(), a.end());

        int poc = 0;
        vector<vector<int> > h(n);
        REP(i,n)
        {
            FOR(j,a[i].second-1,a[i].second+2)
                if (j >= 0 && j < 30 && q[j] != -1)
                {
                    h[q[j]].push_back(i);
                    h[i].push_back(q[j]);
                    poc++;
                }
            q[a[i].second] = i;
        }

        int res = 3;
        if (poc == 0) res = 1;
        else
        {
            vector<bool> f(n, false), bol(n, false);
            bool ok = true;
            REP(j,n) if (!bol[j])
            {
                queue<int> q; q.push(j);
                bol[j] = true;
                for (; !q.empty(); q.pop())
                {
                    int u = q.front();
                    REP(i,h[u].size())
                    {
                        int o = h[u][i];
                        if (!bol[o])
                        {
                            q.push(o);
                            f[o] = !f[u];
                            bol[o] = true;
                        }
                        else if (f[o] == f[u])
                            ok = false;
                    }
                }
            }
            if (ok) res = 2;
        }
        printf("Case #%d: %d\n", sd+1, res);
    }
}
