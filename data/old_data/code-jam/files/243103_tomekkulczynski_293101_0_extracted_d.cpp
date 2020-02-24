//Tomasz Kulczyński
#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <numeric>
#include <cmath>
#include <cstdlib>
using namespace std;

#define X first
#define Y second
#define MP make_pair
#define PB push_back
typedef unsigned long long ll;
typedef double D;
typedef long double ld;
typedef vector<int> VI;
typedef pair<int,int> PII;
#define REP(i,n) for(ll i=0;i<(n);i++)
#define FOR(i,a,b) for(ll i = (a);i<=(b);++i)
#define FORD(i,a,b) for(VAR(i,a);i>=(b);--i)
#define FORE(a,b) for(VAR(a,(b).begin());a!=(b).end();++a)
#define VAR(a,b) __typeof(b) a=(b)
#define SIZE(a) ((int)((a).size()))
#define ALL(x) (x).begin(),(x).end()
#define CLR(x,a) memset(x,a,sizeof(x))
#define COND 1
#define DB(x) { if(COND) { cerr << __LINE__ << " " << #x << " " << x << endl; } }

ll odw(ll x)
{
    ll s = 0;
    while(x)
    {
        s = 10*s + x%10;
        x/=10;
    }
    return s;
}

vector<ll> p;
#define QQ ((ll)(1e13 + 0.1))

int main()
{
    REP(i,10) p.push_back(i);
    FOR(i,1,9) p.push_back(11*i);
    FOR(i,1,9) REP(j,10) p.push_back(101*i+10*j);
    FOR(i,10,99)                        p.push_back(100*i+odw(i));
    FOR(i,10,99) REP(j,10)              p.push_back(1000*i + 100*j + odw(i));
    FOR(i,100,999)                      p.push_back(1000*i+odw(i));
    FOR(i,100,999) REP(j,10)            p.push_back(10000*i + 1000*j + odw(i));
    FOR(i,1000,9999)                    p.push_back(10000*i+odw(i));
    FOR(i,1000,9999) REP(j,10)          p.push_back(100000*i + 10000*j + odw(i));
    FOR(i,10000,99999)                  p.push_back(100000*i+odw(i));
    FOR(i,10000,99999) REP(j,10)        p.push_back(1000000*i + 100000*j + odw(i));
    FOR(i,100000,999999)                p.push_back(1000000*i+odw(i));
    FOR(i,100000,999999) REP(j,10)      p.push_back(10000000*i + 1000000*j + odw(i));
    //REP(i,1000) printf("%llu\t",p[i]);
    ll ma =0;
    REP(i,p.size()-1) ma = max(ma,p[i+1]-p[i]);
    int dd;
    scanf("%d",&dd);
//    printf("przerwa %llu\n",ma);
    FOR(cas,1,dd)
    {
        ll l,r;
        scanf("%llu %llu",&l,&r);
        printf("Case #%d: ",(int)cas);
        ll ret = 0, bla = 0;
        int i=0,j=0;
        while(i < (int)p.size() && p[i]<l) i++;
        while(j+1<(int)p.size() && p[j+1] <= r) j++;
//        printf("(%lld %lld)\n",p[i],p[j]);
        {
            ll a=0,b=0;
            while(i<(int)p.size() && i<=j)
            {
                ret += (r-p[j]) * (r-p[j]+1) / 2;
                ret += a * (r-p[j]);
                a += r-p[j];
                ret += b;
                
                bla += ret / QQ;
                ret %= QQ;

                a++;
                swap(a,b);
                r=p[j]-1;
                j--;
            }
            l--;
            ret += (r-l) * (r-l+1) / 2;
            ret += a* (r-l);
            bla += ret / QQ;
            ret %= QQ;
            if(!bla)
                printf("%llu\n",ret);
            else printf("%llu%013llu\n",bla,ret);
        }
    }
    return 0;
}
