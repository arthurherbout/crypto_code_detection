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

#define INF 1000000000
#define MP make_pair
#define FI first
#define SE second
template<class t> void setmin(t &a, t b){a=min(a,b);}

bool check(int a, string A){
	for(int i = 0; i<A.length(); i++){
		int d = a%10;
		a/=10;
		if(A[A.length()-1-i]!='?'&&'0'+d!=A[A.length()-1-i])
			return false;
	}
	return true;
}

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		string C,J; char c[10000],d[10000];scanf("%s %s ",c,d);C=c;J=d;
		int L = C.length();
		int TOP = 1;
		RE(i,L)
			TOP*=10;
		pair<int,ii> R=MP(INF,MP(0,0));
		RE(a,TOP)RE(b,TOP){
			if(!check(a,C)||!check(b,J))
				continue;
			R=min(R,MP(abs(a-b),MP(a,b)));
		}
		int H = C.length(), G = J.length();int g = R.SE.FI,h=R.SE.SE;
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
		if(R.SE.FI!=0)printf("%d ",R.SE.FI);
		else printf(" ");
		RE(i,H){
			printf("0");
		}
		if(R.SE.SE!=0)printf("%d\n",R.SE.SE);
		else printf("\n");
	}
	return 0;
}
