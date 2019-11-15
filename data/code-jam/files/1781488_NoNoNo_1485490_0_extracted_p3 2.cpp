#include<cstdio>
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<list>
#include<queue>
#include<set>
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

const int MAXN=110;
LL a[MAXN],b[MAXN];
int ta[MAXN],tb[MAXN];
LL odp[MAXN][MAXN];
int n,m;

void readData(){
    scanf("%d%d",&n,&m);
    REP(i,n){
        scanf("%lld%d",&a[i],&ta[i]);
    }
    REP(i,m){
        scanf("%lld%d",&b[i],&tb[i]);
    }
}

void clear(){
    REP(i,MAXN){
        REP(j,MAXN){
            odp[i][j]=0;
        }
    }
}

void licz(){
    LL sum=0;
    odp[n][m]=0;
    FORD(i,n-1,0){
        FORD(j,m-1,0){
            odp[i][j]=odp[i+1][j];
            sum=0;
            FOR(x,j,m-1){
                if (tb[x]==ta[i]){
                    sum+=b[x];
                }
                odp[i][j]=max(odp[i][j],odp[i+1][x]+min(sum,a[i]));
            }
        }
    }
}

int t;

int main(){
    scanf("%d",&t);
    REP(xx,t){
        readData();
        clear();
        licz();
        printf("Case #%d: %lld\n",xx+1, odp[0][0]);
    }
}


