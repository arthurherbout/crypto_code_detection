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

int A[1000005];

int reverse(int n){
	vi V;
	while(n){
		V.push_back(n%10);
		n/=10;
	}
	int R=0;
	FE(w,V){
		R*=10;
		R+=*w;
	}
	return R;
}

int main(){
// 	printf("%d %d %d %d\n",reverse(1),reverse(15),reverse(32000),reverse(5010));
	fill_n(A,1000005,INF);
	A[1]=1;
	REP(n,1,1000000){
		int m=reverse(n);
		A[m]=min(A[m],A[n]+1);
		A[n+1]=min(A[n+1],A[n]+1);
	}
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		int N;scanf("%d ",&N);
		printf("%d\n",A[N]);
	}
	return 0;
}