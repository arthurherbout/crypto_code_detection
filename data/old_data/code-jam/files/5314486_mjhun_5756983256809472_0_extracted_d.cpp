#include <bits/stdc++.h>
#define pb push_back
#define mp make_pair
#define fst first
#define snd second
#define fore(i,a,b) for(int i=a,ThxDem=b;i<ThxDem;++i)
using namespace std;
typedef long long ll;

int ns,nt;

vector<int> g[128];
int mt[128];
bool vis[128];

int go(int x){
	if(vis[x])return 0;
	vis[x]=true;
	for(int y:g[x])if(mt[y]<0||go(mt[y])){mt[y]=x;return 1;}
	return 0;
}
int mmm(){
	int r=0;
	memset(mt,-1,sizeof(mt));
	fore(i,0,ns){
		memset(vis,false,sizeof(vis));
		r+=go(i);
	}
	return r;
}

char b[128][128];
int n,m,l;
int si[128],sj[128],ti[128],tj[128];
bool qwe[128][128];
vector<int> w[128][128];
int d[128][128];

queue<pair<int,int> > q;
vector<pair<int,int> > sol;

bool doit(int mm){
	sol.clear();
	fore(i,0,n)fore(j,0,m)w[i][j].clear();
	memset(qwe,false,sizeof(qwe));
	fore(i,0,ns)g[i].clear();
	fore(_,0,nt){
		if(mm&(1<<_)){
			int ii=ti[_],jj=tj[_];
			int i=ii,j=jj;
			while(i<n&&b[i][j]!='#')w[i][j].pb(_),i++;
			i=ii;j=jj;
			while(i>=0&&b[i][j]!='#')w[i][j].pb(_),i--;
			i=ii;j=jj;
			while(j<m&&b[i][j]!='#')w[i][j].pb(_),j++;
			i=ii;j=jj;
			while(j>=0&&b[i][j]!='#')w[i][j].pb(_),j--;
		}
		else {
			int ii=ti[_],jj=tj[_];
			int i=ii,j=jj;
			while(i<n&&b[i][j]!='#')w[i][j].pb(_),i++;
			i=ii;j=jj;
			while(i>=0&&b[i][j]!='#')w[i][j].pb(_),i--;
			i=ii;j=jj;
			while(j<m&&b[i][j]!='#')w[i][j].pb(_),j++;
			i=ii;j=jj;
			while(j>=0&&b[i][j]!='#')w[i][j].pb(_),j--;
		}
	}
	fore(_,0,ns){
		int i=si[_],j=sj[_];
		memset(d,-1,sizeof(d));
		d[i][j]=0;
		q.push(mp(i,j));
		while(!q.empty()){
			int i=q.front().fst,j=q.front().snd;q.pop();
			for(int x:w[i][j])g[_].pb(x);
			if(d[i][j]==l||qwe[i][j])continue;
			vector<pair<int,int> > ss;
			if(i>0)ss.pb(mp(i-1,j));
			if(i<n-1)ss.pb(mp(i+1,j));
			if(j>0)ss.pb(mp(i,j-1));
			if(j<m-1)ss.pb(mp(i,j+1));
			for(auto p:ss){
				int ii=p.fst,jj=p.snd;
				if(b[ii][jj]=='#')continue;
				if(d[ii][jj]<0){
					d[ii][jj]=d[i][j]+1;
					q.push(mp(ii,jj));
				}
			}
		}
	}
	if(mmm()!=__builtin_popcount(mm))return false;
	fore(i,0,nt)if(mt[i]>=0)sol.pb(mp(mt[i],i));
	return true;
}

int main(){
	int tn;
	scanf("%d",&tn);
	fore(tc,1,tn+1){
		printf("Case #%d: ",tc);
		scanf("%d%d%d",&m,&n,&l);
		fore(i,0,n)scanf("%s",b[i]);
		ns=nt=0;
		fore(i,0,n)fore(j,0,m){
			if(b[i][j]=='S')si[ns]=i,sj[ns++]=j;
			else if(b[i][j]=='T')ti[nt]=i,tj[nt++]=j;
		}
		int r=0;
		fore(k,1,1<<nt)if(__builtin_popcount(k)>__builtin_popcount(r)){
			if(doit(k))r=k;
		}
		if(!r)puts("0");
		else {
			printf("%d\n",(int)__builtin_popcount(r));
			assert(doit(r));
			fore(i,0,sol.size())printf("%d %d\n",sol[i].fst+1,sol[i].snd+1);
		}
	}
	return 0;
}
