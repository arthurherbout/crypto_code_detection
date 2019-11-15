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

int n, d, rows, cols;
lint val[205][205];
int dx[]={0,0,1,-1};
int dy[]={1,-1,0,0};
const int mod = 1e9 + 7;

bool inside(int x, int y) {
	return (x>=0 && y>=0 && x<rows && y<cols);
}

void solve() {
	read4(rows, cols, n, d);
	HEAP<pair<lint, PII> > que;
	FOR(i,0,rows) FOR(j,0,cols) val[i][j] = -1;
	FOR(i,0,n) {
		make3(x, y, z);
		val[x-1][y-1] = z;
		que.push(mp(-z,mp(x-1,y-1)));
	}
	while (!que.empty()) {
		lint z = que.top().st;
		PII para = que.top().nd;
		int x = para.st, y = para.nd;
		que.pop();
		FOR(j,0,4) {
			int nx = x + dx[j];
			int ny = y + dy[j];
			if (inside(nx, ny) && val[nx][ny]==-1) {
				val[nx][ny] = -z+d;
				que.push(mp(-val[nx][ny],mp(nx,ny)));
			}
		}
	}
	FOR(i,0,rows) FOR(j,0,cols) {
		FOR(u,0,4) {
			int nx = i + dx[u];
			int ny = j + dy[u];
			if (inside(nx,ny) && ((val[nx][ny]-val[i][j]>d) || (val[i][j]-val[nx][ny]>d))) {
				printf("IMPOSSIBLE\n");
				return; 
			}
		}
	}
	lint ans = 0;
	FOR(i,0,rows) FOR(j,0,cols) {
		ans += val[i][j];
		ans %= mod;
	}
	printf("%lld\n", ans);
}


int main() {
	make(z);
	FOR(test,1,z+1) {
		printf("Case #%d: ", test);
		solve();
	}
}	
