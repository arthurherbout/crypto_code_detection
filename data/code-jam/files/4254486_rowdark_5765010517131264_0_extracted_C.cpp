#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include<iostream>
#include<ctype.h>
#include<algorithm>
#include<vector>
#include<string>
#include<set>
#include<map>
#include<stack>
#include<queue>
#include<cmath>
#include<bitset>
#include<iomanip>
#include<complex>
#include<utility>

#define X first
#define Y second
#define REP_0(i,n) for(int i=0;i<(n);++i)
#define REP_1(i,n) for(int i=1;i<=(n);++i)
#define REP_2(i,a,b) for(int i=(a);i<(b);++i)
#define REP_3(i,a,b) for(int i=(a);i<=(b);++i)
#define REP_4(i,a,b,c) for(int i=(a);i<(b);i+=(c))
#define DOW_0(i,n) for(int i=(n)-1;-1<i;--i)
#define DOW_1(i,n) for(int i=(n);0<i;--i)
#define DOW_2(i,a,b) for(int i=(b);(a)<i;--i)
#define DOW_3(i,a,b) for(int i=(b);(a)<=i;--i)
#define FOREACH(a,b) for(typeof((b).begin()) a=(b).begin();a!=(b).end();++a)
#define RFOREACH(a,b) for(typeof((b).rbegin()) a=(b).rbegin();a!=(b).rend();++a)
#define PB push_back
#define PF push_front
#define MP make_pair
#define IS insert
#define ES erase
#define IT iterator
#define RI reserve_iterator
#define PQ priority_queue
#define LB lower_bound
#define UB upper_bound
#define ALL(x) x.begin(),x.end()

#define PI 3.1415926535897932384626433832795
#define EXP 2.7182818284590452353602874713527

using namespace std;

typedef long long LL;
typedef long double LD;
typedef double DB;
typedef pair<int,int> PII;
typedef vector<int> VI;
typedef pair<int,PII> PIII;
typedef pair<LD,int> PLDI;
typedef vector<PII> VII;

template<class T>
T Mul(T x,T y,T P){
	T F1=0;
	while(y)
	{
		if(y&1)
		{
			F1+=x;
			if(F1<0||F1>=P)F1-=P;
		}
		x<<=1;
		if(x<0||x>=P)x-=P;
		y>>=1;
	}
	return F1;
}

template<class T>
T Pow(T x,T y,T P){
	T F1=1;x%=P;
	while(y)
	{
		if(y&1)
		{
			F1=Mul(F1,x,P);
		}
		x=Mul(x,x,P);
		y>>=1;
	}
	return F1;
}

template<class T>
T Gcd(T x,T y){
	if(y==0)return x;
	T z;
	while(z=x%y){
		x=y,y=z;
	}
	return y;
}

template<class T>
void UpdateMin(T &x,T y){
	if(y<x)
	{
		x=y;
	}
}

template<class T>
void UpdateMax(T &x,T y){
	if(x<y)
	{
		x=y;
	}
}

template<class T>
T Sqr(const T x){
	return x*x;
}

template<class T>
T Abs(const T x){
	return x<0?-x:x;
}

#define MaxBuffer 20000000
class ReadBuffer{
	private:
	char buff[MaxBuffer];
	char *buf;
	public:
	void init(int size=MaxBuffer)
	{
		fread(buff,1,size,stdin);
		buf=buff;
	}
	template<class T>
	bool readInteger(T &x)
	{
		x=0;
		while(*buf&&isspace(*buf)) ++buf;
		if(*buf==0) return false;
		static bool flag;
		flag=0;
		if(*buf=='-') flag=true;
		else x=*buf-'0';
		while(isdigit(*++buf)) x=x*10+*buf-'0';
		if(flag) x=-x;
		return true;
	}
	template<class T>
	bool readFloat(T &x)
	{
		long double nowpos=0.1;
		x=0;
		while(*buf&&isspace(*buf)) ++buf;
		if(*buf==0) return false;
		static bool flag,decimal;
		decimal=flag=0;
		if(*buf=='-') flag=true,++buf;
		else if(*buf=='.') decimal=true;
		while(isdigit(*buf)||*buf=='.')
		{
			if(*buf=='.') decimal=true;
			else
			{
				if(decimal)
				{
					x+=nowpos*(*buf-'0');
					nowpos*=0.1;
				}
				else
				{
					x=x*10+*buf-'0';
				}
			}
			++buf;
		}
		return true;
	}
	bool readChar(char c)
	{
		if(*buf==0) return 0;
		return c=*buf++,1;
	}
	bool readString(char *s)
	{
		while(*buf&&isspace(*buf)) ++buf;
		if(!*buf) return false;
		while(!isspace(*buf)) *s++=*buf++;
		*s++=0;
		return true;
	}
	int countSpacetonext(){
		int total=0;
		while(*buf&&*buf==' ') ++total,++buf;
		return total;
	}
	bool splitBycharactor(char *s,char Split='\n'){
		while(*buf&&*buf!=Split) *s++=*buf++;
		*s++=0;
		return *buf!=0;
	}
};

struct EDGE{
	int T;EDGE *Nxt;
};

int T,N,Y;

LD dp[101][101][101][2];
int P[501];
int S[501];
vector<pair<int,int> > LS, RS;

int main(){
	freopen("C-small-attempt0.in","r",stdin);
	freopen("c.out","w",stdout);
	scanf("%d",&T);
	REP_1(tt,T){
		scanf("%d%d",&Y,&N);
		LS.clear();
		RS.clear();
		LS.PB(MP(10000,0));
		RS.PB(MP(10000,0));
		REP_1(i,N){
			scanf("%d",P+i);
		}
		REP_1(i,N){
			scanf("%d",S+i);
		}
		REP_1(i,N){
			if(P[i]<0){
				LS.PB(MP(S[i],-P[i]));
			}
			else{
				RS.PB(MP(S[i],P[i]));
			}
		}
		sort(LS.begin(),LS.end());
		reverse(LS.begin(),LS.end());
		{
			int j=1;
			int B=LS[0].second;
			for(int i=1;i<int(LS.size());++i) if(LS[i].second>B){
				 LS[j++]=LS[i];
			}
			LS.resize(j);
		}
		sort(RS.begin(),RS.end());
		reverse(RS.begin(),RS.end());
		{
			int j=1;
			int B=RS[0].second;
			for(int i=1;i<int(RS.size());++i) if(RS[i].second>B){
				 RS[j++]=RS[i];
			}
			RS.resize(j);
		}
		REP_0(i,int(LS.size())) REP_0(j,int(RS.size())) REP_0(k,N+1) REP_0(l,2){
			dp[i][j][k][l]=1e100;
		}
		dp[0][0][0][0]=0;
		REP_0(i,int(LS.size())) REP_0(j,int(RS.size())) REP_0(k,N+1) REP_0(l,2) if(dp[i][j][k][l]<1e50){
			LD npos,ntime=dp[i][j][k][l];
			if(l==0){//LS
				npos=-(LS[k].first*ntime+LS[k].second);
			}
			else{
				npos=RS[k].first*ntime+RS[k].second;
			}
			if(i+1!=(int)LS.size()){
				LD ftime=ntime+(npos+LS[i+1].first*ntime+LS[i+1].second)/(Y-LS[i+1].first);
				LD fpos=-LS[i+1].first*ftime-LS[i+1].second;
				int w=i+1;
				while(w+1!=(int)LS.size()&&LS[w+1].first*ftime+LS[w+1].second>=fpos){
					++w;
				}
				UpdateMin(dp[w][j][i+1][0],ftime);
			}
			if(j+1!=(int)RS.size()){
				LD ftime=ntime+(RS[j+1].first*ntime+RS[j+1].second-npos)/(Y-RS[j+1].first);
				LD fpos=RS[j+1].first*ftime+RS[j+1].second;
				int w=j+1;
				while(w+1!=(int)RS.size()&&RS[w+1].first*ftime+RS[w+1].second<=fpos){
					++w;
				}
				UpdateMin(dp[i][w][j+1][1],ftime);
			}
		}
		LD ans=1e100;
		REP_0(k,N+1) REP_0(l,2) if(dp[LS.size()-1][RS.size()-1][k][l]<1e50){
			UpdateMin(ans,dp[LS.size()-1][RS.size()-1][k][l]);
		}
		cout<<fixed<<setprecision(20)<<"Case #"<<tt<<": "<<ans<<endl;
	}
	return 0;
}
