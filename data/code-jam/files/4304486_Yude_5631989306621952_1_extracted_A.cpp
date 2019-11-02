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
string s;
int main(){
    if (LOCALTEST) {
        freopen("A-large.in", "r", stdin);
        freopen("A-large.out", "w", stdout);
    }
    int t;
    cin >> t;
    int case_n = 1;
    while (t--) {
        cin >> s;
        string ans;
        ans.append(1, s[0]);
        REPP(i,1,SZ(s)) {
            string s1; s1.append(1, s[i]); s1 += ans;
            string s2 = ans; s2.append(1, s[i]);
            if (s1 > s2) ans = s1;
            else ans = s2;
        }
        printf("Case #%d: %s\n", case_n++, ans.c_str());
    }
    return 0;
}
