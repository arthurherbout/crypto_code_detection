#include <algorithm>
#include <iostream>
#include <sstream>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <set>
#include <map>
using namespace std;

#define _ (size_t)
#define all(a) a.begin(), a.end()
#define forint(i, a, b) for(int i=int(a); i<=int(b); ++i)
#define forintdown(i, a, b) for(int i=int(a); i>=int(b); --i)
#define forn(i, n) forint(i, 0, (n)-1)
#define forndown(i, n) forintdown(i, (n)-1, 0)
#define fillchar(a, x) memset(a, x, sizeof(a))
#define fi first
#define se second
#define PB push_back
#define MP make_pair
#ifdef DEBUG
    #define _debug(args...) { fprintf(stderr, args); fflush(stderr); }
#else
    #define _debug(args...) {}
#endif
typedef long long LL;
typedef pair<LL,LL> PLL;
typedef pair<int,int> PII;
typedef pair<double,double> PDD;
typedef vector<LL> VL;
typedef vector<int> VI;
typedef vector<PII> VPI;
typedef vector<string> VS;
template<class T> bool setmax(T &_a, T _b) { if(_b>_a) { _a=_b; return true; } return false; }
template<class T> bool setmin(T &_a, T _b) { if(_b<_a) { _a=_b; return true; } return false; }
template<class T> T gcd(T _a, T _b) { return _b==0?_a:gcd(_b,_a%_b); }

int a[(1<<20)+100];
int n;

void init() {
    int m; cin>>m;
    static int p[55555], f[55555];
    forint(i, 1, m) cin>>p[i];
    forint(i, 1, m) cin>>f[i];

    int t=0;
    forint(i, 1, m) forn(j, f[i]) a[++t]=p[i];
    n=0; while((1<<n)<t) ++n;
    assert((1<<n)==t);
}

void solve(int cs) {
    printf("Case #%d: ", cs); fflush(stdout);

    multiset<int> s(a+1, a+(1<<n)+1);
    s.erase(s.find(0));
    int ans[55];
    forint(i, 1, n) {
        ans[i]=*s.begin();
        //printf("ans[%d]=%d\n", i,ans[i]);
        forn(st, 1<<(i-1)) {
            int cur=ans[i];
            forint(j, 1, i-1) if(st&(1<<(j-1))) cur+=ans[j];
            s.erase(s.find(cur));
        }
    }

    forint(i, 1, n) printf("%d ", ans[i]); printf("\n");
}

int main() {
    int csn; scanf("%d", &csn);
    forint(cs, 1, csn) {
        init();
        solve(cs);
    }
    return 0;
}
