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
#include <cassert>
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


bool clear[1000][1000];
bool clear2[1000][1000];
void scase() {
    int C, X, N;
    scanf("%d%d%d",&C,&X,&N);
    
    REP(i,X+1)REP(j,N) clear[i][j] = true;
    
    REP(i,C) {
        int s,e,t;
        scanf("%d%d%d",&s,&e,&t);
        if (e == N) e = 0;
        if (s == N) s = 0;
        
        while (s != e) {
            clear[t][s] = false;
            clear2[t+1][s] = false;
            s = (s + 1) % N;
            ++t;
        }
        clear[t][s] = false;
    }
    
    int result = 0;
    REP(i,X)REP(j,N) {
        if (!clear[i][j]) continue;
        
        int s = j, t = i;
        while (t < X && clear[t+1][(s+N-1)%N]&& clear2[t+1][(s+N-1)%N]) {
            ++t;
            s = (s + N - 1) % N;
        }
    
        result = max(result, t - i);
    }
    
    printf("%d\n", result);
}

int main() {
    int C;
    scanf("%d",&C);
    FOR(i,1,C+1) {
        printf("Case #%d: ", i);
        scase();
    }
}  
