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
typedef long long LL;
typedef pair<LL,LL> PLL;
typedef pair<int,int> PII;
typedef pair<double,double> PDD;
typedef vector<LL> VL;
typedef vector<int> VI;
typedef vector<PII> VPI;
typedef vector<string> VS;
template<class T> void setmax(T &a, T b) { if(b>a) a=b; }
template<class T> void setmin(T &a, T b) { if(b<a) a=b; }
template<class T> T gcd(T a, T b) { return b==0?a:gcd(b,a%b); }

const LL MOD=LL(1e9)+7;
LL inv(LL x) {
    LL y=MOD-2, ans=1;
    for(; y>0; y>>=1, x=x*x%MOD)
        if(y&1) ans=ans*x%MOD;
    return ans;
}

int get(int s, int i) {
    return (s>>(i*2))&3;
}

int chk(int m, int lnks, int s, int t) {
    forn(i, m) {
        int x=(lnks>>i)&1;
        int y=(get(s,i)==get(t,i));
        if(x!=y) return -1;
    }
    int p=0;
    forn(i, m) {
        int r=get(t,i);
        if((lnks>>i)&1) --r;
        if(get(t,(i+1)%m)==get(t,i)) --r;
        if(get(t,(i+m-1)%m)==get(t,i)) --r;
        if(r<0 || r>1) return -1;
        p|=(r<<i);
    }
    return p;
}

LL go(int n, int m) {
    //printf("go %d %d\n",n,m);fflush(stdout);
    VI all;
    forn(s, 1<<(m*2)) {
        bool b=true;
        forn(i, m)
            if(get(s, i)==0)
                b=false;
        if(b) all.PB(s);
    }

    static LL f[7][1<<6][1<<12];
    fillchar(f, 0);
    f[0][0][0]=1;
    forint(row, 1, n) {
        forn(s1, 1<<m) forn(s2, 1<<(m*2)) if(f[row-1][s1][s2]) {
            for(int cur: all) {
                int t1=chk(m, s1, s2, cur);
                if(t1<0) continue;
                (f[row][t1][cur]+=f[row-1][s1][s2]) %=MOD;
            }
        }
    }
    LL ans=0;
    forn(s2, 1<<(m*2))
        (ans+=f[n][0][s2]) %=MOD;
    return ans;
}

LL ans[111][111];
void pre() {
    LL x[111][111];
    forint(i, 1, 6) forint(j, 1, 6) 
        x[i][j]=go(i, j);
    forint(i, 1, 6) forint(j, 1, 6) {
        LL s=0;
        forint(k, 1, j) s+=x[i][gcd(k,j)];
        s=(s*inv(j))%MOD;
        ans[i][j]=s;
    }
}

void solve(int cs) {
    int n, m; scanf("%d%d", &n,&m);
    printf("Case #%d: ", cs);
    printf("%d\n", (int)ans[n][m]);
}

int main() {
    pre();
    //printf("done\n");
    int csn; scanf("%d", &csn);
    forint(cs, 1, csn) {
        solve(cs);
    }
    return 0;
}
