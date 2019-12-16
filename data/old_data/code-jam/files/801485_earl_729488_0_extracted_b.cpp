#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

const int NIL = (-1);

typedef unsigned U;
typedef long long LL;
typedef long double LD;
typedef unsigned long long UL;

typedef vector<int> VI;
typedef vector<VI> VII;
typedef vector<long long> VLL;
typedef pair<int,int> PII;

#define REP(i,n) for(int i=0; i<(n); i++)
#define FOR(i,b,e) for (int i=(b); i<=(e); i++)
#define FORD(i,b,e) for (int i=(b); i>=(e); i--)
#define FORALL(i,c) for (__typeof((c).begin()) i=(c).begin(); i!=(c).end(); ++i)
#define FOREACH FORALL
#define SIZE(c) ((int)((c).size()))
#define LEN(c) ((int)((c).length()))
#define ALL(c) (c).begin(),(c).end()
#define pb push_back
#define mp make_pair
#define st first
#define nd second
#define IT iterator
#define BC __builtin_popcount

const int N = 1000;

int n;
VI g[N];

int best,s,target;
bool used[N];

void rec(int u) {
    FOREACH(i,g[u]) {
        if (*i==target) {
            best = max(best,s);
        } else {
            if (!used[*i]) {
                used[*i]=1;
                s++;
                rec(*i);
                s--;
                used[*i]=0;
            }
        }
    }
}

inline void single_case(int case_number) {
    scanf("%d",&n);
    REP(i,n) g[i].clear();
    REP(i,3) REP(j,3) if (i!=j) g[i].pb(j);
    REP(i,n-3) {
        int a,b;
        scanf("%d%d",&a,&b);
        a--; b--;
        g[a].pb(i+3);
        g[b].pb(i+3);
        g[i+3].pb(a);
        g[i+3].pb(b);
    }
    REP(i,n) used[i] = 0;
    best = 1;
    REP(i,n) {
        s = 1;
        used[i] = 1;
        target = i;
        rec(target);
        used[i] = 0;
    }
    printf("%d\n",best);
}

int main() {
	int j = 1;
	scanf("%d",&j);//*/
	REP(i,j) {
        printf("Case #%d: ",1+i);   
        single_case(i);
    }
	return 0;
}

