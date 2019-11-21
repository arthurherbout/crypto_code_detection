/*
Date: 2016/08/05 12:15:04 Friday
Author: 
*/
#include<bits/stdc++.h>
using namespace std;
#define X first
#define Y second
#define mp make_pair
#define pb push_back
#define rep(i,a,n) for(int i=(a);i<=(n);++i)
#define dep(i,a,n) for(int i=(a);i>=(n);--i)
#define eps 1e-4
#define pi 3.1415926535897
#define sqr(x) ((x)*(x))
#define MAX(a,b) a=max(a,b)
#define MIN(a,b) a=min(a,b)
#define SZ(x) ((int)(x).size())
#define CPY(a,b) memcpy(a,b,sizeof(a))
#define CLR(a) memset(a,0,sizeof(a))
#define POSIN(x,y) (1<=(x)&&(x)<=n&&1<=(y)&&(y)<=m)
#define all(x) (x).begin(),(x).end()
#define COUT(S,x) cout<<fixed<<setprecision(x)<<S<<endl
#define buli(x) (__builtin_popcountll(x))
#define bur0(x) (__builtin_ctzll(x))
#define bul2(x) (63-__builtin_clzll(x))
#define pw(x) ((ll(1))<<(x))
#define upmo(a,b) (((a)=((a)+(b))%mo)<0?(a)+=mo:(a))
#define mmo(a,b) (((a)=1ll*(a)*(b)%mo)<0?(a)+=mo:(a))
#define y0 y0z
#define y1 y1z
#define yn ynz
#define j0 j0z
#define j1 j1z
#define jn jnz
#define tm tmz
#ifdef LOCAL
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) 
#endif
typedef long long ll;
typedef long double lf;
typedef pair<int,int> pii;
typedef pair<ll,ll> pll;
typedef pair<lf,lf> pff;
typedef complex<double> CD;
const int inf=0x3f3f3f3f;
const int mo=1000000007;
inline void gn(long long&x){
	int sg=1;char c;while(((c=getchar())<'0'||c>'9')&&c!='-');c=='-'?(sg=-1,x=0):(x=c-'0');
	while((c=getchar())>='0'&&c<='9')x=x*10+c-'0';x*=sg;
}
inline void gn(int&x){long long t;gn(t);x=t;}
inline void gn(unsigned long long&x){long long t;gn(t);x=t;}
inline void gn(double&x){double t;scanf("%lf",&t);x=t;}
inline void gn(long double&x){double t;scanf("%lf",&t);x=t;}
template<class T>inline void gn(T&a,T&b){gn(a);gn(b);}
template<class T>inline void gn(T&a,T&b,T&c){gn(a);gn(b);gn(c);}
template<class T>inline void gn(T&a,T&b,T&c,T&d){gn(a);gn(b);gn(c);}
inline void gs(char *s){scanf("%s",s);}
inline void gc(char &c){while((c=getchar())>126 || c<33);}
inline void pc(char c){putchar(c);}
const int DX[]={1,0,-1,0},DY[]={0,1,0,-1};
ll powmod(ll a,ll b) {ll res=1;a%=mo;for(;b;b>>=1){if(b&1)res=res*a%mo;a=a*a%mo;}return res;}
ll powmod(ll a,ll b,ll mo) {ll res=1;a%=mo;for(;b;b>>=1){if(b&1)res=res*a%mo;a=a*a%mo;}return res;}
ll gcd(ll a,ll b) { return b?gcd(b,a%b):a;}
//*******************************************

const int N=111111,M=111111;
int l,m,n,t,C,r;
struct A{lf d,l,r;}a[N];
bool operator<(const A &a,const A &b){return a.d<b.d;}
set<pff>S;
int main(){
#ifdef LOCAL
	freopen("C.in","r",stdin);freopen("C.out","w",stdout);
#endif
	gn(C);rep(_,1,C){
		printf("Case #%d: ",_);
		gn(n,r);
		S.clear();S.insert(mp(-1e30,1e30));
		m=0;
		rep(i,1,n)rep(j,1,n)if(i>1||j>1){
			lf R=r,dis=sqrt(sqr(i-1)+sqr(j-1))*1000000;
			lf a0=asin(R/dis),a1=asin(1000000.0*(i-1)/dis);
			a[++m]={dis,a1-a0,a1+a0};
		}
		sort(a+1,a+1+m);
		int ans=0;
		//if(m>10000){puts("?");continue;}
		/*
		rep(i,1,m){
			lf L=a[i].l,R=a[i].r;
			rep(j,1,m)if(a[j].d<a[i].d){
				if(a[j].l<L)MAX(L,a[j].r);
				if(a[j].r>R)MIN(R,a[j].l);
			}
			ans+=L<R;
		}printf("%d\n",ans);continue;
		*/
		rep(i,1,m){
			auto x=--S.lower_bound(mp(a[i].l,a[i].r));
			if(x->Y<a[i].l-eps*(a[i].r-a[i].l)){
				++x;
				if(x->X>a[i].r+eps*(a[i].r-a[i].l))continue;
				++ans;
				pff t=*x;S.erase(x);
				if(t.Y>a[i].r+eps*(a[i].r-a[i].l))S.insert(mp(a[i].r,t.Y));
				if(t.X<a[i].l-eps*(a[i].r-a[i].l))S.insert(mp(t.X,a[i].l));
				continue;
			}
			++ans;
			pff t=*x;S.erase(x);
			S.insert(mp(t.X,a[i].l));
			if(t.Y>a[i].r+eps*(a[i].r-a[i].l))S.insert(mp(a[i].r,t.Y));
		}printf("%d\n",ans);
	}
	return 0;
}
