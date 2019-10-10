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
const int SIZE = 210;
const int INF = 0x3f3f3f3f;
#define LOCALTEST 1 // change this to 1 to perform localtest on "in.txt"
#define DEBUG 0
double P[SIZE];
int main(){
    if (LOCALTEST) {
        if (!DEBUG) {
            freopen("B-small-attempt0.in", "r", stdin);
            freopen("B-small-attempt0.out", "w", stdout);
        } else {
            freopen("in.txt", "r", stdin);
            freopen("out.txt", "w", stdout);
        }
    }
    CASET {
        int N, K;
        scanf("%d%d", &N, &K);
        REP(i,N) scanf("%lf", &P[i]);

        double maxp = 0.0;

        REP(S1, 1<<N) {
            if (ONES(S1) == K) {
                vector<int> v;
                REP(i, N) if ((1<<i) & S1) v.PB(i);
                // got the chosen members
                REP(S2, 1<<K) {
                    if (ONES(S2) == (K/2)) {
                        double curp1 = 1.0;
                        double curp2 = 1.0;

                        REP(i,K) {
                            if ((1<<i) & S2) curp1 *= P[v[i]], curp2 *= 1.0 - P[v[i]];
                            else curp1 *= 1.0 - P[v[i]], curp2 *= P[v[i]];
                        }

                        smax(maxp, curp1+curp2);

                    }
                }
            }
        }

        printf("Case #%d: %.9f\n", case_n++, maxp);
    }
    return 0;
}
