#include<cmath>
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<iostream>
#include<cstdio>
using namespace std;

#define FOR(i,a,b) for(int i = (a); i < (b); ++i)
#define REP(i,n) FOR(i,0,n)
#define FORE(it,x) for(typeof(x.begin()) it=x.begin();it!=x.end();++it)
#define pb push_back
#define all(x) (x).begin(),(x).end()
#define CLEAR(x,with) memset(x,with,sizeof(x))
#define sz size()
typedef long long ll;

struct Range
{
    double lo, hi;
    Range(double a, double b)
    {
        if(a > b) swap(a, b);
        lo = a;
        hi = b;
    }
};
bool disjoint(const Range& a, const Range& b)
{
    return a.hi <= b.lo || b.hi <= a.lo;
}
Range intersect(const Range& a, const Range& b)
{
    return Range(max(a.lo, b.lo), min(a.hi, b.hi));
}

int n;
int x[1010], y[1010], z[1010], p[1010];

bool disjoint(int a, int b, double P)
{
    double d = abs(x[a] - x[b]) + abs(y[a] - y[b]) + abs(z[a] - z[b]);
    double r = P * p[a] + P * p[b];
    return d > r;    
}

int main()
{
    int cases;
    cin >> cases;
    REP(cc,cases)
    {
        cin >> n;
        REP(i,n)
            cin >> x[i] >> y[i] >> z[i] >> p[i];
        double lo = 0, hi = 0;
        REP(i,n)
        {
            double cand = (abs(x[i]-x[0]) + abs(y[i]-y[0]) + abs(z[i]-z[0])) / double(p[i]);
            hi >?= cand;
        }
        REP(iter,100)
        {
            double mid = (lo+hi)/2.0;
            bool ok = true;
            REP(i,n) 
            {
                REP(j,i)
                    if(disjoint(i, j, mid))
                    {
                        ok = false; 
                        break;
                    }
                if(!ok) break;
            }
            if(ok)
                hi = mid;
            else
                lo = mid;
        }        
        printf("Case #%d: %.8lf\n", cc+1, hi);
    }
}

