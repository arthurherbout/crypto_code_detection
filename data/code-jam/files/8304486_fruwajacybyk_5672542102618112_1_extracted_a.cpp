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
#define jeb fflush(stdout)
 
 
using namespace std;
 
#define max_n 100015

int ans;
int n;
set<VI> secik[110];
char s[15];
int fact[11];


void solve() {
	scanf("%s", s);
	n = strlen(s);
	FOR(i,0,100) secik[i].clear();
	VI w;
	FOR(i,0,n) w.pb(s[i]-'0');
	ans = 1;
	int sum = 0;
	FOR(i,0,n) sum += w[i];
	if (sum > n) {
		printf("1\n");
		return;
	}
	secik[sum].insert(w);
	FOR(j,sum,n+1) {
		set<VI> nowe = secik[j];
		set<VI> nnowe;
		while (1) {
			if (nowe.empty()) break;
			FORE(u, nowe) {
				int suma = 0;
				FOR(jj,0,n) suma += (*u)[jj];
				VI x;
				int sumka = 0;
				int kutas = 0;
				FOR(jj,0,n) {
					FOR(k,0,(*u)[jj]) x.pb(jj+1);
				}
				while (x.size()!=n) x.pb(0);
				sort(all(x));
				FOR(j,0,n) sumka += x[j];
				if (sumka > n) {
					secik[sumka].insert(x);	
				} else {
					do {
							if (sumka == j && secik[j].find(x) == secik[j].end()) nnowe.insert(x);
							if (sumka != j) secik[sumka].insert(x);
					} while (next_permutation(all(x)));
				}
			}
			FORE(i,nnowe) secik[j].insert(*i);
			nowe = nnowe;
			nnowe.clear();
		}
	}
	int ans = 0;
	FOR(i,0,n+1) ans += secik[i].size();
	FOR(j,n+1,100) {
		FORE(u, secik[j]) {
			if (*u!=w) {
				int ile = fact[n];
				int mam[10];
				FOR(xxx,0,10) mam[xxx] = 0;
				FORE(y, *u) {
					mam[*y]++;
				}
				FOR(xxx,0,10) ile/= fact[mam[xxx]];
				ans += ile;
			}
		}
	}
	printf("%d\n", ans);
}


int main() {
	fact[0] = 1;
	FOR(i,1,10) fact[i] = i * fact[i-1];
	make(z);
	FOR(test,1,z+1) {
		printf("Case #%d: ", test);
		solve();
	}
}	
