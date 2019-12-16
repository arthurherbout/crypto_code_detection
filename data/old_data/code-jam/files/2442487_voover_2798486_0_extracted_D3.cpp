#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <cmath>
#include <cstring>
#include <string>
#include <iostream>
#include <complex>
#include <sstream>
using namespace std;
 
typedef long long LL;
typedef unsigned long long ULL;
typedef long double LD;
typedef vector<int> VI;
typedef pair<int,int> PII;
 
#define REP(i,n) for(int i=0;i<(n);++i)
#define SIZE(c) ((int)((c).size()))
#define FOR(i,a,b) for (int i=(a); i<(b); ++i)
#define FOREACH(i,x) for (__typeof((x).begin()) i=(x).begin(); i!=(x).end(); ++i)
#define FORD(i,a,b) for (int i=(a)-1; i>=(b); --i)
#define ALL(v) (v).begin(), (v).end()
 
#define pb push_back
#define mp make_pair
#define st first
#define nd second

LL A,B,N1,N2,V1,V2;
LL Y,X,Vy,Vx;
bool swp = false;

double pos[20];

LL last1[2000000], last2[2000000];

string names[] = {"LEFT", "RIGHT"};

void scase() {
    scanf("%lld%lld%lld%lld%lld%lld%lld%lld%lld%lld",&A,&B,&N1,&N2,&V1,&V2,&Y,&X,&Vy,&Vx);
    swp = false;
    if (Vx == 0 || Vy == 0) {
        printf("DRAW\n");
        return;
    }
    if (Vx < 0) {
        Vx = -Vx;
        X = B - X;
        swap(N1,N2);
        swap(V1,V2);        
    }
    
    if (Vy < 0) {
        Y = A - Y;
        Vy = -Vy;
    }
    
    A *= Vx;
    B *= Vx;
    X *= Vx;
    Y *= Vx;    

    LL T = B / Vx;
    LL Dy = Vy * T;

    LL _y = Y + (B - X) / Vx * Vy;

    REP(i,N1) last1[i] = -1;
    REP(i,N2) last2[i] = -1;

    _y = _y % (2 * A);

    REP(r,10000000) {
        LL y = _y >= A ? 2 * A - _y : _y;
//        printf("%d %lf\n",r,y);
    
        if (r % 2 == 0) {//2
            int wh = (r/2)%N2;
            if (last2[wh] >= 0 && abs(last2[wh] - y) > V2 * 2 * T * N2) {
                printf("%s %d\n", names[(r%2)^swp].c_str(), r/2);
                return;            
            }
            last2[wh] = y;
        } else {
            int wh = (r/2)%N1;
            if (last1[wh] >= 0 && abs(last1[wh] - y) > V1 * 2 * T * N1) {
                printf("%s %d\n", names[(r%2)^swp].c_str(), r/2);
                return;            
            }
            last1[wh] = y;        
        }
    
        _y = (_y + Dy) % (2 * A);
    }
    printf("DRAW\n");
}

int main() {
    int C;
    scanf("%d",&C);
    FOR(i,1,C+1) {
        printf("Case #%d: ", i);
        scase();
    }
}  
