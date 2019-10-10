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
const int N = 32;
const int J = 500;
#define LOCALTEST 1 // change this to 1 to perform localtest on "in.txt"
#define DEBUG 0
int divs[J][11];
int modpow(int a, int p, int m) {
    a %= m;
    int r = 1;
    while (p) r = (p&1) ? r * a : r, a = (LL)a*a%m, p >>= 1;
    return r;
}
int ok(int mask, int base) {
    REP(z,100) {
        int div = 2*z+3;
        int r = (modpow(base, N-1, div) + 1) % div;
        REP(i,N-2) {
            if (mask&(1<<i)) {
                r += modpow(base, i+1, div); r %= div;
            }
        }
        if (r % div == 0) return div;
    }
    return 0;
}
int main(){
    if (LOCALTEST) {
        freopen("in.txt", "r", stdin);
        freopen("C-large.out", "w", stdout);
    }
    DRI(t);
    while (t--) {
        MS0(divs);
        printf("Case #1:\n");

        vector<int> v;
        REP(i,N-2) {
            REPP(j,i+1,N-2) {
                REPP(k,j+1,N-2) {
                    REPP(l,k+1,N-2) {
                        int mask = (1<<i) + (1<<j) + (1<<k) + (1<<l);
                        if (ok(mask, 2) && ok(mask, 4) && ok(mask, 6) && ok(mask, 8) && ok(mask, 10)) {
                            REPP(foo,1,6) divs[SZ(v)][2*foo] = ok(mask,2*foo);
                            v.PB(mask);
                        }
                        if (SZ(v) == J) break;
                    }
                    if (SZ(v) == J) break;
                }
                if (SZ(v) == J) break;
            }
            if (SZ(v) == J) break;
        }

        REP(i,SZ(v)) {
            printf("1");
            vector<int> vp;
            int x = v[i];
            while (x) {
                vp.PB(x & 1);
                x >>= 1;
            }
            while (SZ(vp) < N-2) vp.PB(0);
            PER(j,SZ(vp)-1) printf("%d", vp[j]);
            printf("1");

            REP(j,9) printf(" %d", j%2==0?divs[i][j+2]:2);
            printf("\n");
        }
    }
    return 0;
}
