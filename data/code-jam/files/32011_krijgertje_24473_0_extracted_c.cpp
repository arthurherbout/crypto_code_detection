#include <algorithm> 
#include <iostream> 
#include <sstream> 
#include <string> 
#include <vector> 
#include <queue> 
#include <set> 
#include <map> 
#include <cstdio> 
#include <cstdlib> 
#include <cctype> 
#include <cmath> 
#include <list> 
using namespace std; 

#define PB push_back 
#define MP make_pair 
#define SZ(v) ((int)(v).size()) 
#define FOR(i,a,b) for(int i=(a);i<int(b);++i) 
#define REP(i,n) FOR(i,0,n) 
#define FORE(i,a,b) for(int i=(a);i<=int(b);++i) 
#define REPE(i,n) FORE(i,0,n) 
#define FORSZ(i,a,v) FOR(i,a,SZ(v)) 
#define REPSZ(i,v) FORSZ(i,0,v) 
#define VAR(a,b) __typeof(b) a=b 
#define FORIT(it,v) for(VAR(it,(v).begin());it!=(v).end();++(it)) 
#define DIST(a,b) ABS((a)-(b)) 
#define BETWEEN(i,a,b) ((a)<=(i)&&(i)<(b)) 
#define SQR(a) ((a)*(a)) 
#define ALL(v) (v).begin(),(v).end() 
#define SORT(v) sort(ALL(v)) 
#define UNIQUE(v) (v).erase(unique(ALL(v)),(v).end()) 
typedef long long ll; 
typedef vector<string> VS; 
typedef vector<VS> VVS; 
typedef vector<int> VI; 
typedef vector<VI> VVI; 
typedef vector<double> VD; 
typedef vector<VD> VVD; 
typedef vector<ll> VLL; 
typedef vector<VLL> VVLL; 
typedef pair<int,int> PII; 
typedef vector<PII> VPII;

int R,C;
int g[49][49];
int ret;

void process(int r,int c,int cnt) {
//	printf("%d %d = %d\n",r,c,cnt);
	if(r==(R-1)/2) ret+=cnt;
	if(c%3==0) {
		for(int nr=r-1;nr<=r+1;++nr) for(int nc=c;nc<=c+1;++nc) {
			if(nr<0||nr>=R||nc<0||nc>=C) continue;
			g[nr][nc]-=cnt;
		}
	}
	if(c%3==1) {
		for(int nr=r-1;nr<=r+1;++nr) for(int nc=c;nc<=c+2;nc+=2) {
			if(nr<0||nr>=R||nc<0||nc>=C) continue;
			g[nr][nc]-=cnt;
		}
	}
}

void run(int casenr) {
	scanf("%d%d",&R,&C);
	REP(r,R) REP(c,C) scanf("%d",&g[r][c]);
	ret=0;
	REP(c,C) {
		if(c%3==2) continue;
		FOR(r,2,R) {
			int cur=g[r-1][c]-g[r-2][c];
			process(r,c,cur);
		}
		for(int r=1;r>=0;--r) {
			int cur=g[r+1][c];
			process(r,c,cur);
		}
	}
//	for(int r=0;r<R;++r) { for(int c=0;c<C;++c) printf("%d ",g[r][c]); puts(""); }
//	REP(r,R) REP(c,C) if(c%3!=2) assert(g[r][c]==0);
	printf("Case #%d: %d\n",casenr,ret);
}

int main() {
	int n; scanf("%d",&n); FORE(i,1,n) run(i);
	return 0;
}
