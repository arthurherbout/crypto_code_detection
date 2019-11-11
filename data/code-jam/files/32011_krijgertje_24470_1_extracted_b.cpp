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

int w,h;
int dx1,dy1,dx2,dy2;
int sx,sy;

bool ok(int x,int y) { return 0<=x&&x<w&&0<=y&&y<h; }

void run(int casenr) {
	scanf("%d%d",&w,&h);
	scanf("%d%d%d%d",&dx1,&dy1,&dx2,&dy2);
	scanf("%d%d",&sx,&sy);
	ll ret;
	if(dx1*dy2==dx2*dy1) {
		ret=0;
		vector<bool> been(h+w,false);
		queue<PII> q;
		been[sx+sy]=true; q.push(MP(sx,sy)); ++ret;
		while(!q.empty()) {
			int x=q.front().first,y=q.front().second; q.pop();
			REP(i,2) {
				int nx=x+(i==0?dx1:dx2),ny=y+(i==0?dy1:dy2);
				if(!ok(nx,ny)||been[nx+ny]) continue;
				been[nx+ny]=true; q.push(MP(nx,ny)); ++ret;
			}
		}
	} else {
		ret=0;
		ll cx1=sx,cy1=sy,cx2=sx,cy2=sy;
		ll ncx1,ncy1,ncx2,ncy2;
		while(true) {
//			printf("(%d,%d) (%d,%d)\n",cx1,cy1,cx2,cy2);
			ll cdx=cx2-cx1,cdy=cy2-cy1;
			ll cur=1+(cdx!=0?cdx/(dx2-dx1):cdy!=0?cdy/(dy2-dy1):0);
			assert(cur>0);
			assert(cx1+(cur-1)*(dx2-dx1)==cx2);
			assert(cy1+(cur-1)*(dy2-dy1)==cy2);
			ret+=cur;
			bool end=false;
			while(true) {
				if(ok(cx1+dx1,cy1+dy1)) { ncx1=cx1+dx1,ncy1=cy1+dy1; break;}
				if(cx1==cx2&&cy1==cy2) {
					if(ok(cx1+dx2,cy1+dy2)) { ncx1=cx1+dx2; ncy1=cy1+dy2; break; }
					else { end=true; break; }
				}
				cx1+=dx2-dx1,cy1+=dy2-dy1;
			}
			while(true) {
				if(ok(cx2+dx2,cy2+dy2)) { ncx2=cx2+dx2,ncy2=cy2+dy2; break;}
				if(cx1==cx2&&cy1==cy2) {
					if(ok(cx2+dx1,cy2+dy1)) { ncx2=cx2+dx1; ncy2=cy2+dy1; break; }
					else { end=true; break; }
				}
				cx2+=dx1-dx2,cy2+=dy1-dy2;
			}
			
			if(end) break;
			cx1=ncx1,cy1=ncy1,cx2=ncx2,cy2=ncy2;
		}
	}
	printf("Case #%d: %I64d\n",casenr,ret);
}

int main() {
	int n; scanf("%d",&n); FORE(i,1,n) run(i);
	return 0;
}
