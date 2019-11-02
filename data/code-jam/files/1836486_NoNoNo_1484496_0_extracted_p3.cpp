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

const int MAXP=22;
const int MAXS=2000002;
VI v[MAXS];
int pot[MAXP];

int a[MAXP];
int n;

void liczpot(){
    pot[0]=1;
    REP(i,MAXP-1){
        pot[i+1]=pot[i]*2;
    }
}

void wypisz(int t){
    REP(i,20){
        if ((t & pot[i])){
            printf(" %d",a[i]);
        }
    }
        printf("\n");
}

void licz(){
    int s;
    REP(i,pot[20]){
        s=0;
        REP(j,20){
            if ((i & pot[j])) {
                s+=a[j];
            }
        }
        v[s].PB(i);
        if (v[s].size()>1){
            wypisz(v[s][0]);
            wypisz(v[s][1]);
            return;
        }
    }
}
int t;

void czysc(){
    REP(i,pot[20]) v[i].clear();
}

int main(){
    liczpot();
    scanf("%d",&t);
    REP(i,t){
        czysc();
        scanf("%d",&n);
        REP(j,20){
            scanf("%d",&a[j]);
        }
        printf("Case #%d:\n",i+1);
        licz();
    }
}
