#include<stdio.h>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<bitset>
#include<utility>
#include<functional>
#include<iomanip>
#include<sstream>
#include<ctime>
#include<cassert>
using namespace std;
#define y0 y0z
#define y1 y1z
#define yn ynz
#define j0 j0z
#define j1 j1z
#define jn jnz
#define tm tmz
#define buli(x) (__builtin_popcountll(x))
#define bur0(x) (__builtin_ctzll(x))
#define bul2(x) (63-__builtin_clzll(x))
#define mp make_pair
#define pb push_back
#define fi first
#define se second
#define fil(a,b) memset((a),(b),sizeof(a))
#define cl(a) fil(a,0)
#define siz(x) ((int)(x).size())
#define all(x) (x).begin(),(x).end()
#define foreach(it,a) for(__typeof((a).begin()) it=(a).begin();it!=(a).end();it++)
#define rep(i,a,b) for (int i=(a),_ed=(b);i<_ed;i++)
#define per(i,a,b) for (int i=(b)-1,_ed=(a);i>=_ed;i--)
#define forg(i,gu) for (int i=gu;~i;i=e[i].next)
#define pw(x) ((ll(1))<<(x))
#define upmo(a,b) (((a)=((a)+(b))%mo)<0?(a)+=mo:(a))
#define mmo(a,b) (((a)=1ll*(a)*(b)%mo)<0?(a)+=mo:(a))
void getre(){int x=0;printf("%d\n",1/x);}
void gettle(){int res=1;while(1)res<<=1;printf("%d\n",res);}
typedef pair<int,int> pii;
typedef vector<int> vi;
typedef vector<pii> vpii;
template<typename T,typename S>inline bool upmin(T&a,const S&b){return a>b?a=b,1:0;}
template<typename T,typename S>inline bool upmax(T&a,const S&b){return a<b?a=b,1:0;}
template<typename N,typename PN>inline N flo(N a,PN b){return a>=0?a/b:-((-a-1)/b)-1;}
template<typename N,typename PN>inline N cei(N a,PN b){return a>0?(a-1)/b+1:-(-a/b);}
template<typename N>N gcd(N a,N b){return b?gcd(b,a%b):a;}
template<typename N>inline int sgn(N a){return a>0?1:(a<0?-1:0);}
#if ( ( _WIN32 || __WIN32__ ) && __cplusplus < 201103L)
#define lld "%I64d"
#else
#define lld "%lld"
#endif
inline void gn(long long&x){
	int sg=1;char c;while(((c=getchar())<'0'||c>'9')&&c!='-');c=='-'?(sg=-1,x=0):(x=c-'0');
	while((c=getchar())>='0'&&c<='9')x=x*10+c-'0';x*=sg;
}
inline void gn(int&x){long long t;gn(t);x=t;}
inline void gn(unsigned long long&x){long long t;gn(t);x=t;}
inline void gn(double&x){double t;scanf("%lf",&t);x=t;}
inline void gn(long double&x){double t;scanf("%lf",&t);x=t;}
inline void gs(char *s){scanf("%s",s);}
inline void gc(char &c){while((c=getchar())>126 || c<33);}
inline void pc(char c){putchar(c);}
#ifdef JCVB
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...)
#endif
typedef long long ll;
typedef double db;
inline ll sqr(ll a){return a*a;}
inline db sqrf(db a){return a*a;}
const int inf=0x3f3f3f3f;
const db pi=3.14159265358979323846264338327950288L;
const db eps=1e-6;
//const int mo=0;
//int qp(int a,ll b){int n=1;do{if(b&1)n=1ll*n*a%mo;a=1ll*a*a%mo;}while(b>>=1);return n;}


struct nfa{
	int n;
	int start;
	bool e[40][40][11];
	int ed;
}emp,ze;


nfa kl(nfa a){
	rep(i,0,a.n)if(pw(i)&a.ed){
		a.e[i][a.start][10]=1;
	}
	a.ed|=pw(a.start);
	return a;
}
nfa comb(nfa a,nfa b){
	if(a.n==0)return b;
	if(b.n==0)return a;
	nfa c=a;
	c.n=a.n+b.n;
	rep(i,0,b.n)rep(j,0,b.n)rep(k,0,11)if(b.e[i][j][k]){
		c.e[i+a.n][j+a.n][k]=1;
	}
	c.ed=0;
	rep(i,0,b.n)if(pw(i)&b.ed)c.ed|=pw(i+a.n);
	rep(i,0,a.n)if(pw(i)&a.ed)c.e[i][b.start+a.n][10]=1;
	return c;
}
nfa orr(vector<nfa> arr){
	if(siz(arr)==1)return arr[0];
	nfa c=emp;

	int cursu=0;
	rep(id,0,siz(arr)){
		rep(i,0,arr[id].n)
			rep(j,0,arr[id].n)
				rep(k,0,11)if(arr[id].e[i][j][k]){
					c.e[i+cursu][j+cursu][k]=1;
				}
		rep(i,0,arr[id].n)if(pw(i)&arr[id].ed)c.ed|=pw(cursu+i);
		cursu+=arr[id].n;
	}
	c.start=cursu++;
	c.n=cursu;
	cursu=0;
	rep(id,0,siz(arr)){
		c.e[c.start][arr[id].start+cursu][10]=1;
		cursu+=arr[id].n;
	}
	return c;
}
nfa str(string s){
	nfa c=emp;
	c.n=siz(s)+1;
	c.start=0;
	c.ed|=pw(siz(s));
	rep(i,0,siz(s)){
		c.e[i][i+1][s[i]-'0']=1;
	}
	return c;
}

string t;
nfa x;


nfa get(string t){
	if(t=="")return ze;
	int n=siz(t);
	assert(n>=1);
	int le=-1,ri=-1;
	int top=0;
	vector<int>tmp;
	tmp.clear();
	rep(i,0,n)
		if(t[i]=='('){
			if(top==0)le=i;
			top++;
		}else if(t[i]==')'){
			top--;
			if(top==0){
				ri=i;
				break;
			}
		}else if(t[i]=='|' && top==1)tmp.pb(i);
	if(le==-1){
		return str(t);
	}
	if(ri!=n-1 && t[ri+1]=='*'){
		nfa cur=ze;
		string lef="",rig="",mid="";
		rep(j,0,le)lef+=t[j];
		cur=comb(cur,get(lef));
		rep(j,le+1,ri)mid+=t[j];
		cur=comb(cur,kl(get(mid)));
		rep(j,ri+2,n)rig+=t[j];
		cur=comb(cur,get(rig));
		return cur;
	}else{
		nfa cur=ze;
		string lef="",rig="",mid="";
		rep(j,0,le)lef+=t[j];
		cur=comb(cur,get(lef));
		tmp.pb(ri);
		vector<nfa>mii;
		int las=le;
		rep(j,0,siz(tmp)){
			mid="";
			rep(tt,las+1,tmp[j])mid+=t[tt];
			mii.pb(get(mid));
			las=tmp[j];
		}
		cur=comb(cur,orr(mii));
		rep(j,ri+1,n)rig+=t[j];
		cur=comb(cur,get(rig));
		return cur;
	}
}

int go[40][11];


int bfs(int u){
	int qu[40];
	int vis[40];
	cl(vis);
	int p=0,q=0;
	qu[q++]=u;
	vis[u]=1;
	while(p!=q){
		int u=qu[p++];
		for (int i=0;i<x.n;i++)if(x.e[u][i][10] && !vis[i]){
			qu[q++]=i;
			vis[i]=1;
		}
	}
	int su=0;
	rep(i,0,x.n)if(vis[i])su|=pw(i);
	return su;
}


int get2(int u,int c){
	int s=go[u][10];
	int ne=0;
	rep(i,0,x.n)if(pw(i)&s){
		rep(j,0,x.n)if(x.e[i][j][c]){
			ne|=pw(j);
		}
	}
	rep(i,0,x.n)if(pw(i)&ne)ne|=go[i][10];
	return ne;
}

int getnext(int s,int c){
	int n=0;
	rep(i,0,x.n)if(pw(i)&s)n|=go[i][c];
	return n;
}
int qu[1111111];int p=0,q=0;

map<int,int>ma;
int idtot;

int to[111111][10];
int term[111111];


ll f[20][2][2][111111];
int dig[20],len;


ll F(int i,int les,int gre,int v){
	if(i==-1)return gre==1 && term[v];
	if(~f[i][les][gre][v])return f[i][les][gre][v];

	ll su=0;
	for (int j=0;j<=(les?9:dig[i]);j++){
		su+=F(i-1,les|(j<dig[i]),gre|(j>0),(gre|(j>0))?to[v][j]:1);
	}
	return f[i][les][gre][v]=su;
}
ll gao(ll m){
	if(m==0)return 0;
	len=0;
	while(m){
		dig[len++]=m%10;
		m/=10;
	}
	fil(f,-1);
	return F(len-1,0,0,1);
}

void main_work(){ //remember to clear all variables
	ll l,r;
	gn(l);gn(r);
	cin>>t;
	x=get(t);
	/*rep(i,0,x.n)rep(j,0,x.n)rep(k,0,11)if(x.e[i][j][k]){
		debug("%d %d (%d)\n",i,j,k);
	}*/
	cl(go);

	rep(i,0,x.n){
		go[i][10]=bfs(i);
	}
	rep(i,0,x.n){
		rep(j,0,10)go[i][j]=get2(i,j);
	}
	cl(to);
	p=q=0;
	idtot=0;
	ma.clear();
	qu[q++]=getnext(pw(x.start),10);
	ma[qu[0]]=++idtot;
	while(p!=q){
		int u=qu[p++];
		rep(j,0,10){
			int v=getnext(u,j);
			if(!ma.count(v)){
				ma[v]=++idtot;
				qu[q++]=v;
			}
			to[ma[u]][j]=ma[v];
		}
	}
	rep(i,1,idtot+1){
		int u=qu[i-1];
		if(u&x.ed)term[i]=1;
		else term[i]=0;
	}
	printf(lld"\n",gao(r)-gao(l-1));
}

int main()
{
#ifdef JCVB
	freopen("1.in","r",stdin);
	freopen("1.out","w",stdout);
	int _time_jc=clock();
#endif
	int tes;gn(tes);
	rep(_,1,tes+1){
		printf("Case #%d: ",_);
		//printf("Case #%d:\n",_);
		main_work();
	}
#ifdef JCVB
	debug("time: %d\n",int(clock()-_time_jc));
#endif
	return 0;
}


