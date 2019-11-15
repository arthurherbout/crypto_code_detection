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

int R,C,W;
int dp[1<<12][1<<12];

int dfs(int a, int b){
	if(dp[a][b])
		return dp[a][b];
	int Count=0;
	RE(i,C-W+1){
		bool ok=true;
		int D=0;
		RE(j,C)
			if(a&(1<<j)){
				if(j>=i&&j<i+W)
					ok&=(bool)(b&(1<<j));
				else
					ok&=(bool)(!(b&(1<<j)));
				D+=(bool)(b&(1<<j));
			}
		Count+=ok;
		if(ok&&D==W)
			return dp[a][b]=0;
	}
	if(Count<1)
		return dp[a][b]=-15;
	int mi=INF;
	RE(i,C)
		if(!(a&(1<<i))){
			int ma=max(dfs(a|(1<<i),b|(1<<i)),dfs(a|(1<<i),b));
			mi=min(mi,ma);
		}
	return dp[a][b]=mi+1;
}

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		scanf("%d %d %d ",&R,&C,&W);
		RE(i,1<<C)RE(j,1<<C)
			dp[i][j]=0;
		if(R==1){
			printf("%d\n",dfs(0,0));
		}
	}
	return 0;
}