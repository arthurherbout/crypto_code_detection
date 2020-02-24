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

int C,D,V;

int dfs(int i, int a, int bi){
	if(i==10)
		return INF;
	bool ok=true;
	RE(j,V+1)
		if(!(bi&(1<<j))){
			ok=false;
			break;
		}
	if(ok)
		return 0;
	int R=INF;
	REP(j,a,V+1){
		int ci=bi;
		BREP(v,V+1,j)
			ci|=((bool)(bi&(1<<(v-j)))<<v);
		R=min(R,dfs(i+1,j+1,ci));
	}
	return R+1;
}

int main(){
	
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		scanf("%d %d %d ",&C,&D,&V);
		int bi=1;
		RE(d,D){
			int j;
			scanf("%d ",&j);
			int ci=bi;
			BREP(v,V+1,j)
				ci|=((bool)(bi&(1<<(v-j)))<<v);
			bi=ci;
		}
		printf("%d\n",dfs(0,1,bi));
	}
	return 0;
}