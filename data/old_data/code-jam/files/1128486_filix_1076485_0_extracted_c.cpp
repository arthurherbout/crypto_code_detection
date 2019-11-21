#include<cstdio>
#include<algorithm>
#include<vector>
#include<list>
#include<stack>
#include<queue>
#include<cmath>
#include<map>
#include<string>
#include<set>
#include<cstring>
#include<iostream>
#include<sstream>
using namespace std;
 
#define PB push_back
#define FORE(i,t) for(typeof(t.begin())i=t.begin();i!=t.end();++i)
#define SZ(x) int((x).size())
#define REP(i,n) for(int i=0,_=(n);i<_;++i)
#define FOR(i,a,b) for(int i=(a),_=(b);i<=_;++i)
#define FORD(i,a,b) for(int i=(a),_=(b);i>=_;--i)

typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> pii;

const int INF=1e9+9;

ll t[10005];

void inline jeden() {
	int n;
	scanf("%d", &n);
	ll l, h;
	scanf("%lld%lld", &l, &h);
	REP (i, n) {
	  scanf("%lld", t + i);
	}
	ll x = l;
	for (bool ok;;) {
	  //printf("\nx = %lld\n", x);
	  ok = true;
	  REP (i, n) {
	    //printf("%lld ", t[i]);
	    if (x > h) {
	      break;
	    }
	    if (x < t[i]) {
	      ll m = t[i] % x;
	      if (m > 0) {
	        ll ile = t[i] / x;
  	      x = max(x + 1, t[i] / ile);
  	      ok = false;
  	    }
	    }
	    if (x > h) {
	      break;
	    }
	    if (x > t[i]) {
	      ll m = x % t[i];
	      if (m > 0) {
	        ll ile = x / t[i] + 1LL;
	        x = t[i] * ile;
	        ok = false;
	        //printf(" dwa %lld\n", x);
	      }
	    }
	  }
	  if (ok || x > h) {
	    break;
	  }
	}
	if (x > h) {
	  puts("NO");
	} else {
	  printf("%lld\n", x);
	}
}

int main() {
	int z;scanf("%d",&z);
	FOR (i, 1, z) {
	  printf("Case #%d: ", i);
	  jeden();
	}
}
