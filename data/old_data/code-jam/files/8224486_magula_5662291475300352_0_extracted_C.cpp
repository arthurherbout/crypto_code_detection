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

LL T[500005],L[500005];

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		int N;scanf("%d ",&N);
		int X=0;
		RE(i,N){
			LL D,H,M;scanf("%lld %lld %lld ",&D,&H,&M);
			RE(h,H){
				T[X]=M+h;
				L[X]=D;
				X++;
			}
		}
		if(X<2){
			printf("0\n");
		}else if(X==2){
			int a=0,b=1;
// 			if(L[a]>L[b])
// 				swap(a,b);
			printf("%d\n",((LL)360-L[a])*T[a]>=((LL)360-L[b]+360)*T[b]||((LL)360-L[b])*T[b]>=((LL)360-L[a]+360)*T[a]);
		}else{
			printf("1000000000\n");
		}
	}
	return 0;
}