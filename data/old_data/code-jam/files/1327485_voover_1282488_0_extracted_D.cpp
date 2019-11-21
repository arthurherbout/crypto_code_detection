// NAGLOWEK

#include <algorithm>
#include <cstdio>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <string>
#include <cstring>
#include <functional>

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

#define pb push_back
#define mp make_pair
#define st first
#define nd second

typedef LL intT;

intT GCD(intT a, intT b) {
	intT tmp;
	while(b!=0) {
		tmp = b;
		b = a%b;
		a = tmp;
	}
	return a;
}

/* gcd = ax - by, x>=0, y>=0 */
intT eGCD(intT a, intT b, intT &x, intT &y) {
    if (b==0) {
        x = intT(1);
        y = intT(0);
        return a;
    }   
    intT d = eGCD(b,a%b,y,x);
    y = a-x*(a/b)-y;
    x = b-x;
    return d;
}

intT inverse(intT a, intT p) { intT x,y; eGCD(a%p+p,p,x,y); return x % p; }

LL gcd(LL a, LL b){
  return b?gcd(b,a%b):a;
}

bool rel[500][500];
bool was[500];
bool used[500];
int result[500];
void scase(int CID){
  int N;
  scanf("%d",&N);
  int order[N];
  REP(i,N)scanf("%d",&order[i]);
  REP(i,N)order[i]--;
  REP(i,N)REP(j,N)rel[i][j] = false;
  REP(i,N)was[i] = used[i] = false;
  REP(i,N)result[i] = 0;
  
  REP(i,N){
    int v = order[i];
    REP(j,v)
      REP(k,j)
        if(was[j] && was[k]){
          rel[k][j] = true;  
        }
    was[v] = true;    
  }
  REP(k,N)REP(i,N)REP(j,N)
    rel[i][j] = rel[i][j] || (rel[i][k]&&rel[k][j]);
  
  REP(i,N){
    if(i == order[N-1]){
      result[i] = 0;
      continue;
    }
    int count = 0;
    FOR(j,i+1,N) if(rel[i][j])++count;
    FORD(k,N,0){
      if(used[k])continue;
      if(count == 0){
        result[i] = k;
        used[k] = true;
        break;
      }else --count;
    }
    FOR(j,i+1,N)
      if(!rel[i][j])
        FOR(k,j+1,N)
          rel[j][k] = true;
  }
  
  printf("Case #%d: ",CID);
  REP(i,N)printf("%d ",result[i]+1);
  printf("\n");
}

int main(){
  int z;
  scanf("%d",&z);
  REP(i,z)scase(i+1);
}

