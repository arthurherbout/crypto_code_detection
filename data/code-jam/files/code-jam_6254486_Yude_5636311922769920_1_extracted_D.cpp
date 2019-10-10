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
const int SIZE = 1e5+10;
const int INF = 0x3f3f3f3f;
#define LOCALTEST 1 // change this to 1 to perform localtest on "in.txt"
#define DEBUG 0
LL k,c;
LL s;
int main(){
    if (LOCALTEST) {
        freopen("D-large.in", "r", stdin);
        freopen("D-large.out", "w", stdout);
    }
    CASET {

        scanf("%lld%lld%lld", &k, &c, &s);
        printf("Case #%d:", case_n++);

        int need = (k + c - 1) / c;
        LL kpowc = 1; REP(i,c) kpowc *= k;
        if (s < need) printf(" IMPOSSIBLE\n");
        else {
            REP(i,need) {
                LL pos = i * c;
                LL ma = kpowc;
                LL ans = 0;
                LL kpow = kpowc;
                REPP(j,1,c+1) {
                    kpow /= k;
                    ans += (pos + j - 1) * kpow;
                    LL removed = k - (pos + j - 1) - 1;
                    ma -= max(0LL, removed * kpow);
                }
                ans++;
                smin(ans, ma);
                printf(" %lld", ans);
            }
            printf("\n");
        }
    }
    return 0;
}
