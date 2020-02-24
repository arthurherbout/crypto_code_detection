// ayy
// ' lamo
#include <bits/stdc++.h>
#include <bits/extc++.h>
using namespace std;
using namespace __gnu_pbds;
typedef long long ll;
typedef long double ld; //CARE
typedef complex<ld> pt;
#define fi first
#define se second
#define pb push_back
const ld eps=1e-8;
const int inf=1e9+99;
const ll linf=1e18+99;
const int P=1e9+7;






char grid[128][128];
bool block[128][128],win[128][128],kills[12][12][1024];
int r,c,m;
vector<pair<int,int> > S,T;
int sn,tn;
const int dx[]={1,0,-1,0};
const int dy[]={0,1,0,-1,0};

bool seen[1024][1024];
int dp[1024][1024];
int g(int sk,int tk,bool pp=0) {
	bool &se=seen[sk][tk];
	int &ans=dp[sk][tk];
	if(se && !pp) return ans;
	se=1;
	ans=0;
	int SC=-1,TC=-1;
	for(int s=0;s<sn;s++) {
		int skk=sk&~(1<<s); if(skk==sk) continue;
		for(int t=0;t<tn;t++) {
			int tkk=tk&~(1<<t); if(tkk==tk) continue;
			if(!kills[s][t][tk]) continue;
			int rec=g(skk,tkk);
			if(rec>=ans) ans=rec+1, SC=s, TC=t;
		}
	}

	if(ans && pp) {
		printf("%d %d\n",SC+1,TC+1);
		int skk=sk&~(1<<SC);
		int tkk=tk&~(1<<TC);
		g(skk,tkk,1);
	}
	return ans;
}
void rz() {
	for(int i=0;i<1024;i++) for(int j=0;j<1024;j++) seen[i][j]=0;
}


bool bfs(int x,int y,int ts) {
	vector<pair<int,int> > V={{x,y}};
	static bool seen[128][128];
	for(int i=1;i<=r;i++) for(int j=1;j<=c;j++) seen[i][j]=0;
	seen[x][y]=1;
	for(;ts--;) {
		vector<pair<int,int> > W=V;
		V.clear();
		for(const auto &P:W) for(int k=0;k<4;k++) {
			if(block[P.fi][P.se]) continue;
			int x=P.fi+dx[k];
			int y=P.se+dy[k];
			if(seen[x][y]) continue;
			if(grid[x][y]=='#') continue;
			seen[x][y]=1;
			V.pb({x,y});
		}
	}
	for(int i=1;i<=r;i++) for(int j=1;j<=c;j++)
		if(seen[i][j] && win[i][j]) return 1;
	return 0;
}
void _m(int t) {
	scanf("%d%d%d",&c,&r,&m);
	for(int i=1;i<=r;i++) scanf("%s",grid[i]+1);
	for(int i=0;i<=r+1;i++) for(int j=0;j<=c+1;j++)
		if(!i||i>r || !j||j>c) grid[i][j]='#';
	S.clear(); T.clear();
	for(int i=1;i<=r;i++) for(int j=1;j<=c;j++)
		if(grid[i][j]=='S') S.pb({i,j});
	for(int i=1;i<=r;i++) for(int j=1;j<=c;j++)
		if(grid[i][j]=='T') T.pb({i,j});
	sn=(int)S.size(); tn=(int)T.size();

	for(int mk=1<<tn;--mk>=0;) {
		for(int t=0;t<tn;t++) for(int k=0;k<4;k++) {
			if(!(mk&(1<<t))) continue;
			int x=T[t].fi,y=T[t].se;
			for(;;) {
				block[x][y]=1;
				x+=dx[k];
				y+=dy[k];
				if(grid[x][y]=='#') break;
			}
		}

		for(int t=0;t<tn;t++) {
			for(int k=0;k<4;k++) {
				if(!(mk&(1<<t))) continue;
				int x=T[t].fi,y=T[t].se;
				for(;;) {
					win[x][y]=1;
					x+=dx[k];
					y+=dy[k];
					if(grid[x][y]=='#') break;
				}
			}

			for(int s=0;s<sn;s++) kills[s][t][mk] = bfs(S[s].fi,S[s].se,m);

			for(int i=1;i<=r;i++) for(int j=1;j<=c;j++) win[i][j]=0;
		}

		for(int i=1;i<=r;i++) for(int j=1;j<=c;j++)
			block[i][j]=0;
	}

	printf("Case #%d: %d\n",t,g((1<<sn)-1, (1<<tn)-1, 0));
	g((1<<sn)-1, (1<<tn)-1, 1);
	rz();
}



int32_t main() {
	// freopen("4.hard.in","r",stdin);
	int T; scanf("%d",&T); for(int t=1;t<=T;t++) _m(t);
}










