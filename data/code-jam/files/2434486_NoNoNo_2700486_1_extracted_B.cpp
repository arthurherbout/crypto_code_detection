#include<cstdio>
#include<iostream>
#include<cassert>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<list>
#include<queue>
#include<set>
#include<cassert>
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
#define MP make_pair
#define PB push_back
#define ST first
#define ND second

typedef long double LD;

int x,y;
int n;
void readData(){
    scanf("%d%d%d",&n,&x,&y);
}
int abs(int x){
    if (x < 0) return -x;
    return x;   
}

LD ans(){
    int kr = 0;
    int obe = 1;
    int k = 0;
    while (n >= obe){
        n -= obe;
        obe += 4;
        kr += 2;
    }
    if (abs(x) + y < kr) return 1.0;
    if (abs(x) + y > kr) return 0.0;
    if (n == 0) return 0.0;
    if (x == 0) return 0.0;
    int h = (obe - 1)/2;
    if (h + y < n) return 1.0;
    k = y + 1; //k potrzebujemy po naszej stronie
    assert(2*h >= n);
    assert(h >= k);
    LD res = 0.0;
    LD nex = 1.0;
    int wyk = n;
    int powt = n - k + 1;
    REP(i, powt){
        
        res += nex;
        nex = (LD)(nex*(n - i))/(i + 1.0);
        while (nex > 10.0){
            nex /= 2;
            res /= 2;
            wyk--;
        }
    }
    while (wyk){
        res /= 2;
        --wyk;
    }
    return res;
}

int main(){
    int t;
    scanf("%d",&t);
    REP(i,t){
        readData();
        printf("Case #%d: %.7Lf\n",i + 1, ans());
    }
}

