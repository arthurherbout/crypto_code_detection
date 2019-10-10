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
const int SIZE = 5000;
const int INF = 0x3f3f3f3f;
#define LOCALTEST 1 // change this to 1 to perform localtest on "in.txt"
#define DEBUG 0
int N, R, P, S;
int numperlevel[3][12][3]; // [PRS at level j][level j][needed number of PRS]
string stringsperlevel[3][12];  // [PRS at level j][level j]
string thestr(int x) {
    if (x == 0) return "P";
    if (x == 1) return "R";
    return "S";
}
int main(){
    if (LOCALTEST) {
        if (!DEBUG) {
            freopen("A-small-attempt0.in", "r", stdin);
            freopen("A-small-attempt0.out", "w", stdout);
        } else {
            freopen("in.txt", "r", stdin);
            freopen("out.txt", "w", stdout);
        }
    }
    MS0(numperlevel);
    REP(i, 3) numperlevel[i][0][i] = 1;

    REP(i, 3) stringsperlevel[i][0] = thestr(i);

    REPP(level, 1, 12) {
        REP(i, 3) {
            int a = i, b = (i+1)%3, c = (i+2)%3;
            numperlevel[a][level][a] = numperlevel[a][level-1][a] + numperlevel[b][level-1][a];
            numperlevel[a][level][b] = numperlevel[a][level-1][b] + numperlevel[b][level-1][b];
            numperlevel[a][level][c] = numperlevel[a][level-1][c] + numperlevel[b][level-1][c];
            //printf("numperlevel[%d][%d][%d] = %d\n", i, level, a, numperlevel[i][level][a]);
            //printf("numperlevel[%d][%d][%d] = %d\n", i, level, b, numperlevel[i][level][b]);
            //printf("numperlevel[%d][%d][%d] = %d\n", i, level, c, numperlevel[i][level][c]);

            string sa = stringsperlevel[a][level-1];
            string sb = stringsperlevel[b][level-1];
            if (sa < sb) {
                stringsperlevel[a][level] = sa + sb;
            } else {
                stringsperlevel[a][level] = sb + sa;
            }
        }
    }

    CASET {
        scanf("%d%d%d%d", &N, &R, &P, &S);



        printf("Case #%d: ", case_n++);
        bool possible = false;
        vector<string> vs;
        REP(i, 3) {
            if (numperlevel[i][N][0] == P && numperlevel[i][N][1] == R && numperlevel[i][N][2] == S) {
                possible = true;
                vs.PB(stringsperlevel[i][N]);
            }
        }
        if (!possible) {
            printf("IMPOSSIBLE\n");
        } else {
            // possible
            sort(ALL(vs));
            printf("%s\n", vs[0].c_str());
        }
    }
    return 0;
}
