#include<cstdio>
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<list>
#include<queue>
#include<set>
#include <cmath>
using namespace std;
typedef vector<int> VI;
typedef long long LL;
#define FOR(x, b, e) for(int x=b; x<=(e); ++x)
#define FORD(x, b, e) for(int x=b; x>=(e); --x)
#define REP(x, n) for(int x=0; x<(n); ++x)
#define VAR(v,n) __typeof(n) v=(n)
#define ALL(c) c.begin(),c.end()
#define SIZE(x) (int)x.size()
#define FOREACH(i,c) for(VAR(i,(c).begin());i!=(c).end();++i)
#define PB push_back
#define ST first
#define ND second

const int MAXN=2010;
const int MAXA=255;

long double t[MAXN],x[MAXN],acc[MAXA];
long double d;
int n,a;
long double tost;

void readData(){
    scanf("%Lf %d %d",&d,&n,&a);
    REP(i,n){
        scanf("%Lf %Lf",&t[i],&x[i]);
    }
    REP(i,a){
        scanf("%Lf",&acc[i]);
    }
}

long double licz(long double ac){
    long double s=0;
    tost=0;
    REP(i,n){
        if (x[i]<=d)
        s=max(s,-sqrt(2*x[i]/ac)+t[i]);
    }
    REP(i,n-1){
        if ((x[i]<=d) && ( d <= x[i+1])) {
            tost=(d-x[i])*(t[i+1]-t[i])/(x[i+1]-x[i])+t[i];
        }
    }
    s=max(s,tost-sqrt(2*d/ac));
    return s+sqrt(2*d/ac);
}

int main(){
    int t;
    scanf("%d",&t);
    REP(rr,t){
        tost=0;
        readData();
        printf("Case #%d: \n",rr+1);
        REP(i,a){
            printf("%.8Lf\n",licz(acc[i]));
        }
    }
}
