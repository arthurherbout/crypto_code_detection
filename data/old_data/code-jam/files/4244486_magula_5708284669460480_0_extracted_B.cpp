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

char A[1000],B[1000],C[1000];
int K,L,S;
int E,F,G;

void dfs(int i){
	if(i==S){
		int I=0;
		RE(j,S-L+1){
			bool o=true;
			REP(k,j,j+L)
				o&=B[k-j]==C[k];
			I+=o;
		}
		E+=I;
		G=max(G,I);
		F++;
		return;
	}
	RE(c,K){
		C[i]=A[c];
		dfs(i+1);
	}
}

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		scanf("%d %d %d ",&K,&L,&S);
		RE(i,K)
			scanf("%c ",&A[i]);
		RE(i,L)
			scanf("%c ",&B[i]);
		E=0;F=0;G=0;
		dfs(0);
		printf("%.8lf\n",(double)G-((double)E)/F);
	}
	return 0;
}