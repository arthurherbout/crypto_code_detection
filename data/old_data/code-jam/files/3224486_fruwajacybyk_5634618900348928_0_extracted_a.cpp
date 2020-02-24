#include <bits/stdc++.h>

#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define FORD(i, a, b) for(int i = (a); i >= (b); --i)
#define VAR(v, i) __typeof(i) v=(i)
#define FORE(i, c) for(VAR(i, (c).begin()); i != (c).end(); ++i)
#define all(v) (v).begin(),(v).end()

#define PII pair<int,int>
#define mp make_pair
#define st first
#define nd second
#define pb push_back
#define lint long long int
#define VI vector<int>

#define debug(x) {cerr <<#x <<" = " <<x <<endl; }
#define debug2(x,y) {cerr <<#x <<" = " <<x << ", "<<#y<<" = "<< y <<endl; } 
#define debug3(x,y,z) {cerr <<#x <<" = " <<x << ", "<<#y<<" = "<< y << ", " << #z << " = " << z <<endl; } 
#define debugv(x) {{cerr <<#x <<" = "; FORE(itt, (x)) cerr <<*itt <<", "; cerr <<endl; }}
#define debugt(t,n) {{cerr <<#t <<" = "; FOR(it,0,(n)) cerr <<t[it] <<", "; cerr <<endl; }}

#define make( x) int (x); scanf("%d",&(x));
#define make2( x, y) int (x), (y); scanf("%d%d",&(x),&(y));
#define make3(x, y, z) int (x), (y), (z); scanf("%d%d%d",&(x),&(y),&(z));
#define make4(x, y, z, t) int (x), (y), (z), (t); scanf("%d%d%d%d",&(x),&(y),&(z),&(t));
#define makev(v,n) VI (v); FOR(i,0,(n)) { make(a); (v).pb(a);} 
#define IOS ios_base::sync_with_stdio(0)
#define HEAP priority_queue

#define read( x) scanf("%d",&(x));
#define read2( x, y) scanf("%d%d",&(x),&(y));
#define read3(x, y, z) scanf("%d%d%d",&(x),&(y),&(z));
#define read4(x, y, z, t) scanf("%d%d%d%d",&(x),&(y),&(z),&(t));
#define readv(v,n) FOR(i,0,(n)) { make(a); (v).pb(a);}


using namespace std;
const int max_n = 1e5+5;

char s[max_n];

void solve() {
		scanf("%s", s);
		int n = strlen(s);
		VI v;
		char last = s[0];
		int ile = 1;
		FOR(i,1,n) {
			if (last == s[i]) ile++;
			else {
				v.pb(ile);
				last = s[i];
				ile = 1;
			}
		}
		v.pb(ile);
		int ans = 0;
		vector<PII> w;
		FOR(i,0,v.size()) {
			ans += (v[i]/2) * 10;
			if(v[i]%2 != 0) w.pb(mp(1,i%2));
			if (w.size() >= 2 && w[w.size()-1].nd == w[w.size()-2].nd) {
				w.pop_back();
				w.pop_back();
				ans += 10;
			}
		}
		ans += (w.size()/2) * 5;
		printf("%d\n", ans);
}


int main() {
	make(tt);
	FOR(test,1,tt+1) {
		printf("Case #%d: ",test);
		solve();
	}
}

	
