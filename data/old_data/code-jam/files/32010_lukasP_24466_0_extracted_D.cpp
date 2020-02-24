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
const int mod = 30031;
void zlepsi(int &a, int b)
{
    a = (a + b) % mod;
}
int main() {
    int tt; scanf("%d", &tt);
    REP(sd,tt)
    {
        int n, k, p; scanf("%d %d %d", &n, &k, &p);
        int b[n + 1][1<<10];
        memset(b, 0x00, sizeof(b));

        int s = 1<<p, w = 1<<k;
        b[k - 1][w - 1] = 1;
        FOR(i,k,n) REP(j,s) if (b[i - 1][j] > 0)
        {
            bitset<12> e(j << 1);
            b[i - 1][j] %= mod;
            e[0] = true;
            if (e[p])
            {
                e[p] = false;
                zlepsi(b[i][e.to_ulong()], b[i - 1][j]);
            }
            else
            {
                REP(l,p) if (e[l+1])
                {
                    e[l+1] = false;
                    zlepsi(b[i][e.to_ulong()], b[i - 1][j]);
                    e[l+1] = true;
                }
            }
        }


        printf("Case #%d: %d\n", sd+1, b[n-1][w - 1] % mod);
    }
}
