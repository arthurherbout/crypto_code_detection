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


const int N = 30;
int n,p[N];
long long f,x[N];

inline long long labs(long long x) { return max(x,-x); }

inline void single_case(int case_number) {
    scanf("%d",&n);
    scanf("%lld",&x[0]);
    n--;
    REP(i,n)
        scanf("%lld",&x[i]);
    scanf("%lld",&f);
    REP(i,n) p[i]=i;
    long long best = NIL;
    do {
        long long l = 0;
        REP(i,n-1) l += labs(x[p[i]]-x[p[i+1]]);
        l += labs(x[p[0]]);
        l += labs(x[p[n-1]]);
        if (l<=f&&l>=best) best = l;
    } while (next_permutation(p,p+n));
    if (best<0) printf("NO SOLUTION\n");
    else printf("%lld\n",best);

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

