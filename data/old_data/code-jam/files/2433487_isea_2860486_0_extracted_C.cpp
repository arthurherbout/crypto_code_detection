// New Ryan
// Create @ 23:35 06-15 2013
// Comment - 

#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <bitset>
#include <queue>
#include <map>
#include <set>

using namespace std;

// Self Template Code BGEIN

#define sz(x) ((int)((x).size()))
#define out(x) printf(#x" %d\n", x)
#define all(x) (x).begin(), (x).end()
#define clz(x) memset (x, 0, sizeof(x))
#define read(x) freopen (x, "r", stdin)
#define wrte(x) freopen (x, "w", stdout)
#define rep(i,n) for (int i = 0; i < (n); ++i)
#define repf(i,a,b) for (int i = (a); i <= (b); ++i)
#define repd(i,a,b) for (int i = (a); i >= (b); --i)
#define repcase int t, Case = 1; for (scanf ("%d", &t); t; --t)
#define repeach(i,x) for (__typeof((x).begin()) i = (x).begin(); i != (x).end(); ++i)

typedef long long int64;
typedef pair<int, int> pii;
typedef pair<int64, int64> pll;
typedef pair<double, double> pdd;

int sgn(double x) { return (x > 1e-8) - (x < -1e-8); }
int count_bit(int x) { return x == 0? 0 : count_bit(x >> 1) + (x & 1); }

template<class T> inline void ckmin(T &a, const T b) { if (b < a) a = b; }
template<class T> inline void ckmax(T &a, const T b) { if (b > a) a = b; }

// Self Template Code END

struct edge {
    int u, v, s1, s2;
    
    void input() {
        scanf ("%d%d%d%d", &u, &v, &s1, &s2);
        --u, --v;
    }
};

const int inf = 9999999;
const int maxn = 1000 + 10;

int n, m, p, g[maxn][maxn], g2[maxn][maxn];
edge e[maxn * 2];

int main() {
    wrte ("C.out");
    
    repcase {
        fprintf (stderr, "Solve Case %d ...\n", Case);
        scanf ("%d%d%d", &n, &m, &p);
        rep (i, n) {
            g[i][i] = g2[i][i] = 0;
            rep (j, i) {
                g[i][j] = g[j][i] = inf;
                g2[i][j] = g2[j][i] = inf;
            }
        }
        rep (i, m) {
            e[i].input();
            ckmin (g[e[i].u][e[i].v], e[i].s1);
            ckmin (g2[e[i].u][e[i].v], e[i].s2);
        }
        
        rep (k, n) rep (i, n) rep (j, n) {
            ckmin (g[i][j], g[i][k] + g[k][j]);
            ckmin (g2[i][j], g2[i][k] + g2[k][j]);
        }
        
        int res = -1;
        int cur = 0;
        rep (i, p) {
            int id ;
            scanf ("%d", &id);
            --id;
            cur += e[id].s1;
            
            //printf ("maxd %d cur %d id %d can %d ? %d\n", g2[0][1], cur, e[id].v, g[e[id].v][1], g2[0][1] - cur);
            
            if (g[e[id].v][1] > g2[0][1] - cur && res == -1) {
                res = id + 1;
            }
            if (g2[0][e[id].v] < cur && res == -1) {
               res = id + 1;
            } 
        }
        
        if (res == -1) {
            printf ("Case #%d: Looks Good To Me\n", Case++);
        } else {
            printf ("Case #%d: %d\n", Case++, res);
        }
    }
    return 0;
}

