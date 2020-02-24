#include <cstdio>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <stack>
#include <cassert>
using namespace std;

typedef long long LL;
typedef vector<int> vi;
typedef pair<int, int> ii;
typedef vector<ii > vii;
typedef queue<int> qi;
typedef set<int> si;

#define RE(i,b) for(int i=0; i<(int)(b); i++)
#define REP(i,a,b) for(int i=(a); i<(int)(b); i++)
#define BREP(i,a,b) for(int i=(a)-1; i>=(b); i--)
#define FE(i,X) for(typeof((X).begin()) i=(X).begin(); i!=(X).end(); ++i)

#define INF 1000000000000000000
#define MP make_pair
#define FI first
#define SE second
template<class t> void setmin(t &a, t b){a=min(a,b);}

// bool check(int a, string A){
// 	for(int i = 0; i<A.length(); i++){
// 		int d = a%10;
// 		a/=10;
// 		if(A[A.length()-1-i]!='?'&&'0'+d!=A[A.length()-1-i])
// 			return false;
// 	}
// 	return true;
// }

int L;
pair<LL,pair<LL,LL> > R;
int off;
int TOP;
string C,J;
int C2[1000],J2[1000];

LL ev(int i){
	if(!i){
		LL a = 0;
		RE(k,L){
			a*=10;
			a+=C2[k];
		}
		return a;
	}
	else{
		LL a = 0;
		RE(k,L){
			a*=10;
			a+=J2[k];
		}
		return a;
	}
}

void dfs(int i){
	if(i==L){
		LL a = ev(0), b = ev(1);
		R=min(R,MP(max(a-b,b-a),MP(a,b)));
		return;
	}
	if(off>0){
		if(C[i]=='?')C2[i]=0;
		if(J[i]=='?')J2[i]=9;
		dfs(i+1);
		off=1;
	}else
	if(off<0){
		if(C[i]=='?')C2[i]=9;
		if(J[i]=='?')J2[i]=0;
		dfs(i+1);
		off=-1;
	}else{
		if(C[i]=='?'&&J[i]=='?'){
			C2[i]=0;
			J2[i]=1;
			off=-1;
			dfs(i+1);
			C2[i]=1;
			J2[i]=0;
			off=+1;
			dfs(i+1);
			C2[i]=0;
			J2[i]=0;
			off=0;
			dfs(i+1);
			off=0;
		}else if(C[i]=='?'){
			C2[i]=J[i]-'0'+0;
			off=0;
			dfs(i+1);
			if(J[i]-'0'<9){
				C2[i]=J[i]-'0'+1;
				off=+1;
				dfs(i+1);
			}
			if(J[i]-'0'>0){
				C2[i]=J[i]-'0'-1;
				off=-1;
				dfs(i+1);
			}
			off=0;
		}else if(J[i]=='?'){
			J2[i]=C[i]-'0'+0;
			off=0;
			dfs(i+1);
			if(C[i]-'0'<9){
				J2[i]=C[i]-'0'+1;
				off=+1;
				dfs(i+1);
			}
			if(C[i]-'0'>0){
				J2[i]=C[i]-'0'-1;
				off=-1;
				dfs(i+1);
			}
			off=0;
		}else{
			off=(C2[i]>J2[i])?+1:((C2[i]==J2[i])?0:-1);
			dfs(i+1);
			off=0;
		}
	}
}

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		char c[10000],d[10000];scanf("%s %s ",c,d);C=c;J=d;
		L = C.length();
		TOP = 1;
		RE(i,L)
			TOP*=10;
		R=MP(INF,MP(0,0));
		off=0;
		RE(k,L){
			if(C[k]!='?')
				C2[k]=C[k]-'0';
			if(J[k]!='?')
				J2[k]=J[k]-'0';
		}
// 		RE(a,TOP)RE(b,TOP){
// 			if(!check(a,C)||!check(b,J))
// 				continue;
// 			R=min(R,MP(abs(a-b),MP(a,b)));
// 		}
		dfs(0);
		int H = C.length(), G = J.length();LL g = R.SE.FI,h=R.SE.SE;
		while(g){
			g/=10;
			G--;
		}
		while(h){
			h/=10;
			H--;
		}
		RE(i,G){
			printf("0");
		}
		if(R.SE.FI!=0)printf("%lld ",R.SE.FI);
		else printf(" ");
		RE(i,H){
			printf("0");
		}
		if(R.SE.SE!=0)printf("%lld\n",R.SE.SE);
		else printf("\n");
	}
	return 0;
}
