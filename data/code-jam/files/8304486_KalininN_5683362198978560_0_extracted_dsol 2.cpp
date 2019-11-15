#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ld = long double;
// using D = double;
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

const int maxn = 205;
const ll inf = 1e18;
const int MOD = 1000000007;

ll zmin[maxn][maxn], zmax[maxn][maxn];
int h, w, n, D;

int main()
{
    int NT = 0;
    scanf("%d", &NT);
    for (int T = 1; T <= NT; T++)
    {
        printf("Case #%d: ", T);
        
        scanf("%d%d%d%d", &h, &w, &n, &D);
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                zmin[i][j] = 0;
                zmax[i][j] = inf;
            }
        }
        for (int i = 0; i < n; i++)
        {
            int x, y, z;
            scanf("%d%d%d", &x, &y, &z);
            x--, y--;
            for (int xx = 0; xx < h; xx++)
            {
                for (int yy = 0; yy < w; yy++)
                {
                    ll cd = (ll)D * (abs(xx - x) + abs(yy - y));
                    zmin[xx][yy] = max(zmin[xx][yy], z - cd);
                    zmax[xx][yy] = min(zmax[xx][yy], z + cd);
                }
            }
        }
        bool ok = true;
        ll answer = 0;
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                if (zmin[i][j] > zmax[i][j]) ok = false;
                answer += zmax[i][j];
            }
        }
        if (!ok) printf("IMPOSSIBLE\n");
        else printf("%lld\n", answer % MOD);
        fprintf(stderr, "%d/%d cases done!\n", T, NT);
    }
    return 0;
}
