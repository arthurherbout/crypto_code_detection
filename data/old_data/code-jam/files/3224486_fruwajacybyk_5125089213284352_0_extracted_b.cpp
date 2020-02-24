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
const int max_n = 2e5+5;
int n;
int par[max_n];
VI ch[max_n];
char let[max_n];
int licz[10];
char s[10][max_n];
char chuj[205];
int sz[205];

bool szukaj(int p) {
	int zz = strlen(s[p]);
	for(int j=0;j+zz<=n;j++) {
		bool ok = true;
		FOR(k,0,zz) {
			if(s[p][k]!=chuj[j+k]) {
				ok = false;
				break;
			}
		}
		if (ok) return true;
	}
	return false;
}

void dfs(int v) {
	sz[v] = 1;
	FORE(i, ch[v]) {
		dfs(*i);
		sz[v] +=sz[*i]; 
	}
}

void solve() {
	read(n);
	FOR(i,0,n+1) ch[i].clear();
	FOR(i,1,n+1) {
		make(a); par[i] = a;
		ch[a].pb(i);
	}
	scanf("%s", let);
	make(m);
	FOR(i,0,m) scanf("%s", s[i]);
	FOR(j,0,m) licz[j] = 0;
	dfs(0);	
	VI act = ch[0];
	FOR(i,0,100000) {
		act = ch[0];
		FOR(j,0,n) {
			int sum = 0;
			FOR(u,0,act.size()) sum += sz[act[u]];
			int z = rand() % sum;
			sum = 0;
			int choosen = 0;
			FOR(u,0,act.size()) {
				int po = sum + sz[act[u]];
				if (z>= sum && z < po) {
					choosen = act[u];
					swap(act[u], act[act.size()-1]);
					act.pop_back();
					break;
				}else sum = po;
			}
			chuj[j] = let[choosen-1];
			FORE(u, ch[choosen]) act.pb(*u);
		}
		FOR(j,0,m) {
			if (szukaj(j)) licz[j]++;
		}
	}
	FOR(j,0,m) {
		printf("%.10lf ", licz[j]*0.00001);
	}
	printf("\n");


	
}


int main() {
	srand(time(NULL));
	make(tt);
	FOR(test,1,tt+1) {
		printf("Case #%d: ",test);
		solve();
	}
}

	
