#include <string>
#include <cctype>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <cstdlib>
#include <cstdio>
#include <queue>
#include <stack>
#include <memory.h>
#include <assert.h>
using namespace std;
#define SZ(a) (int)(a).size()
#define FOR(i,a,b) for (int i=(a); i<=(b); ++i)
#define FORD(i,a,b) for (int i=(a); i>=(b); --i)
#define REP(i,n) for (int i=0; i<(n); ++i)
#define ALL(c) c.begin(), c.end()
#define TR(container, it) for(typeof(container.begin()) it = container.begin(); it != container.end(); it++)
#define CONTAIN(container, it) (container.find(it)!=container.end())
#define CLR(c,n) memset(c,n,sizeof(c))
#define MCPY(a,b) memcpy(a,b,sizeof(b))
#define MP make_pair
#define PB push_back
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<string> VS;
typedef pair<int, int> PII;
typedef istringstream ISS;
const int INF=0x3F3F3F3F;
const double EPS=1e-9;
const double PI=acos(-1);
VS readline() {VS ret; string s; getline(cin,s); ISS is(s); while (is>>s) ret.PB(s); return ret;}
VI readvi() {VS l=readline(); VI ret; REP(i,SZ(l)) ret.PB(atoi(l[i].c_str()));return ret;}
int readint() {return readvi()[0];}
int R, C, n;
char g[16][16];
long ID(int *s) {
	sort(s, s+n);
	long r=0;
	REP(i,n) r|=(long)s[i]<<(8*i);
	return r;
}
int dx[4]={-1,0,1,0}, dy[4]={0,-1,0,1};
bool connect(int *x, int *y) {
	bool c[5][5]; CLR(c,0);
	REP(i,n) REP(j,n) {
		int sx=abs(x[i]-x[j]), sy=abs(y[i]-y[j]);
		if (sx+sy<=1) c[i][j]=true;
	}
	REP(k,n) REP(i,n) REP(j,n) if (c[i][k]&&c[k][j]) c[i][j]=true;
	REP(i,n) if (!c[0][i]) return false;
	return true;
}
bool move(int *s, int id, int dir) {
	int x[5], y[5];
	REP(i,n) x[i]=s[i]/16, y[i]=s[i]%16;
	//REP(i,n) cerr << x[i] << "," << y[i] << " "; cerr << endl;
	bool before=connect(x,y);
	int x2=x[id]-dx[dir], y2=y[id]-dy[dir];
	x[id]+=dx[dir]; y[id]+=dy[dir];
	if (x[id]<0||x[id]>=R||y[id]<0||y[id]>=C) return false;
	if (x2<0||x2>=R||y2<0||y2>=C) return false;
	if (g[x[id]][y[id]]=='#') return false;
	if (g[x2][y2]=='#') return false;
	REP(i,n) if (id!=i&&x[i]==x[id]&&y[i]==y[id]) return false;
	REP(i,n) if (id!=i&&x[i]==x2&&y[i]==y2) return false;
	bool after=connect(x,y);
	if (!before&&!after) return false;
	s[id]=x[id]*16+y[id];
	return true;
}
int go(long from, long to) {
	map<long,int> ans; ans[from]=0;
	int x[4], y[4], id[5], t[5];
	queue<long> q; q.push(from);
	while (!q.empty()) {
		from=q.front(); q.pop();
		int step=ans[from];
		if (from==to) return step;
		REP(i,n) {
			id[i]=from&255;
			from>>=8;
		}
		//REP(i,n) cerr << id[i] << " "; cerr << endl;
		REP(i,n) REP(j,4) {
			REP(k,n) t[k]=id[k];
			if (!move(t,i,j)) continue;
			long cur=ID(t);
			if (!CONTAIN(ans,cur)) {
				ans[cur]=step+1;
				q.push(cur);
			}
		}
	}
	return -1;
}
int main()
{
	//freopen("A.in","r",stdin);
	//freopen("A-small-attempt0.in", "r", stdin); freopen("A-small-attempt0.out", "w", stdout);
	//freopen("A-small-attempt1.in", "r", stdin); freopen("A-small-attempt1.out", "w", stdout);
	freopen("A-large.in", "r", stdin); freopen("A-large.out", "w", stdout);
	int testCase=readint();
	for (int caseID=1; caseID<=testCase; ++caseID) {
		cerr << caseID << " of " << testCase << endl;
		cin>>R>>C; REP(i,R) cin>> g[i];
		int from[5], to[5], nf=0, nt=0;
		REP(i,R) REP(j,C) {
			if (g[i][j]=='o') {
				g[i][j]='.';
				from[nf++]=i*16+j;
			} else if (g[i][j]=='x') {
				g[i][j]='.';
				to[nt++]=i*16+j;
			} else if (g[i][j]=='w') {
				g[i][j]='.';
				from[nf++]=i*16+j;
				to[nt++]=i*16+j;
			}
		}
		assert(nf==nt);
		n=nf;
		printf("Case #%d: %d\n", caseID, go(ID(from),ID(to)));
	}
}
