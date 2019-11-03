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

int S[1000005];
int M[1000005];

vi G[1000005];
int E=0;
bool vis[1000005];

void dfs(int v){
	if(vis[v])
		return;
	vis[v]=true;
	E--;
	FE(w,G[v])
		dfs(*w);
}

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		int N,D;scanf("%d %d ",&N,&D);
		RE(i,N)	G[i].clear();
		int S0,As,Cs,Rs;
		scanf("%d %d %d %d ",&S0,&As,&Cs,&Rs);
		int M0,Am,Cm,Rm;
		scanf("%d %d %d %d ",&M0,&Am,&Cm,&Rm);
		int T=0;
		RE(i,N){
			S[i]=S0;
			T=max(T,S[i]);
			S0=(S0*As+Cs)%Rs;
		}
		RE(i,N){
			M[i]=(i?(M0%i):-1);
// 			printf("%d\n",M[i]);
			if(i)
				G[M[i]].push_back(i);
			M0=(M0*Am+Cm)%Rm;
		}
		int F=0;
		REP(l,0,Rs){
			E=0;
			fill_n(vis,N,false);
			bool ok=true;
			RE(i,N){
				if(S[i]-l>D||S[i]<l)
					dfs(i);
			}
			if(ok)F=max(F,N+E);
		}
		printf("%d\n",F);
	}
	return 0;
}