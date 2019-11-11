#include <bits/stdc++.h>
#define SZ(X) ((int)(X).size())
#define ALL(X) (X).begin(), (X).end()
#define REP(I, N) for (int I = 0; I < (N); ++I)
#define REPP(I, A, B) for (int I = (A); I < (B); ++I)
#define PER(I, N) for (int I = (N); I >= 0; --I)
#define PERR(I, A, B) for (int I = (A); I >= B; --I)
#define ITR(I, A) for (__typeof((A).begin()) I=(A).begin(), _##i=(A).end(); I!=_##i; ++I)
#define RI(X) scanf("%d", &(X))
#define RII(X, Y) scanf("%d%d", &(X), &(Y))
#define RIII(X, Y, Z) scanf("%d%d%d", &(X), &(Y), &(Z))
#define DRI(X) int (X); scanf("%d", &X)
#define DRII(X, Y) int X, Y; scanf("%d%d", &X, &Y)
#define DRIII(X, Y, Z) int X, Y, Z; scanf("%d%d%d", &X, &Y, &Z)
#define RS(X) scanf("%s", (X))
#define CASET int ___T, case_n = 1; scanf("%d ", &___T); while (___T-- > 0)
#define MP make_pair
#define PB push_back
#define MS0(X) memset((X), 0, sizeof((X)))
#define MS1(X) memset((X), -1, sizeof((X)))
#define LEN(X) strlen(X)
#define PII pair<int,int>
#define VPII vector<pair<int,int> >
#define PLL pair<long long,long long>
#define F first
#define S second
#define LB(X) __builtin_ctz((X))
#define ONES(X) __builtin_popcount((X))
typedef long long LL;
using namespace std;
template <class T>  inline void smax(T &x,T y){ x = max((x), (y));}
template <class T>  inline void smin(T &x,T y){ x = min((x), (y));}
const int MOD = 1e9+7;
const int SIZE = 1e3+10;
const int INF = 0x3f3f3f3f;
const int N = 32;
const int J = 500;
#define LOCALTEST 1 // change this to 1 to perform localtest on "in.txt"
#define DEBUG 0
int n;
int bff[SIZE];
int vis[SIZE];
int reverselen[SIZE];
int special[2];
vector<vector<int> > backwardbff;
int reversedfs(int u) {
    int r = 1;
    REP(i,SZ(backwardbff[u])) {
        int v = backwardbff[u][i];
        if (v == bff[u]) continue;
        smax(r, reversedfs(v) + 1);
    }
    return r;
}
int dfs(int u, int v, int sz) {
    if (vis[u]) {
        if (u == v) return sz;
        else return 0;
    }
    vis[u] = 1;
    return dfs(bff[u], v, sz+1);
}
int main(){
    if (LOCALTEST) {
        if (!DEBUG) {
            freopen("C-large.in", "r", stdin);
            freopen("C-large.out", "w", stdout);
        } else {

            freopen("in.txt", "r", stdin);
            freopen("out.txt", "w", stdout);
        }
    }
    CASET {
        RI(n);
        backwardbff.clear();
        backwardbff = vector<vector<int> >(n+1);
        REP(i,n) {
            RI(bff[i+1]);
            backwardbff[bff[i+1]].PB(i+1);
        }
        MS0(reverselen);
        REPP(i,1,n+1) {
            if (i == bff[bff[i]])
                reverselen[i] = reversedfs(i) - 1;
            if (DEBUG) {
                printf("reverselen[%d] = %d\n", i, reverselen[i]);

            }
        }
        int ans = 0;
        REPP(i,1,n+1) {
            MS0(vis);
            smax(ans, dfs(i,i,0));
        }
        int sumspecial = 0;
        MS0(vis);
        REPP(i,1,n+1) {
            if (vis[i]) continue;
            if (i == bff[bff[i]]) {
                vis[i] = vis[bff[i]] = 1;
                sumspecial += reverselen[i] + reverselen[bff[i]] + 2;
            }
        }
        smax(ans, sumspecial);
        printf("Case #%d: %d\n", case_n++, ans);
    }
    return 0;
}
