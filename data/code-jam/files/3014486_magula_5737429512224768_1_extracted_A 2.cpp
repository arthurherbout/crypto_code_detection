#include "stdio.h"
#include "string"
#include "algorithm"
#include "math.h"
#include "vector"
#include "set"
#include "queue"
#include "map"
#include "stack"
using namespace std;

typedef long long LL;
typedef vector<int> vi;
typedef pair<int, int> ii;
typedef vector<ii > vii;
typedef queue<int> qi;
typedef set<int> si;

#define RE(i,b) for(int i=0; i<(int)(b); i++)
#define REP(i,a,b) for(int i=(int)(a); i<(int)(b); i++)
#define BREP(i,a,b) for(int i=(int)(a)-1; i>=(int)(b); i--)
#define FE(i,X) for(typeof((X).begin()) i=(X).begin(); i!=(X).end(); ++i)

#define INF 2000000000
#define MP make_pair
#define FI first
#define SE second

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		int N,X;scanf("%d %d ",&N,&X);
		multiset<int> V;
		RE(n,N){
			int a;scanf("%d ",&a);
			V.insert(-a);
		}
		int P=0;
		while(V.size()){
			int a=-*V.begin();
			V.erase(V.begin());
			P++;
			if(V.lower_bound(-(X-a))!=V.end())
				V.erase(V.lower_bound(-(X-a)));
		}
		printf("%d\n",P);
	}
	return 0; 
}