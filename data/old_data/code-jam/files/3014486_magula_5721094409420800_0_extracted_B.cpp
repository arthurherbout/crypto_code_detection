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

int A[1005];int N;bool S[1005];int Erg;

void visit(int i){
	if(i==N){
		int P=0;
		RE(a,N)
		REP(b,a+1,N){
			if((S[a]==1&&S[b]==0)||(S[a]==0&&S[b]==0&&A[a]>A[b])||(S[a]==1&&S[b]==1&&A[a]<A[b]))
				P++;
		}
		Erg=min(P,Erg);
		return;
	}
	S[i]=0;
	visit(i+1);
	S[i]=1;
	visit(i+1);
}

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		scanf("%d ",&N);
		Erg=INF;
		RE(n,N)
			scanf("%d ",&A[n]);
		visit(0);
		printf("%d\n",Erg);
	}
	return 0; 
}