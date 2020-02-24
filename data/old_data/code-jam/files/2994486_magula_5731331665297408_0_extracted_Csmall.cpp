#include "stdio.h"
#include "string"
#include "algorithm"
#include "math.h"
#include "vector"
#include "set"
#include "queue"
#include "map"
#include "stack"		
#include "stack"		
#include "stack"		
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

#define INF 2000000000
#define MP make_pair
#define FI first
#define SE second

int A[100];
bool G[100][100];

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		int N,M;scanf("%d %d ",&N,&M);
		RE(n,N)
			scanf("%d ",&A[n]);
		RE(i,N)
		RE(j,N)
			G[i][j]=false;
		RE(m,M){
			int a,b;scanf("%d %d ",&a,&b);a--;b--;
			G[a][b]=G[b][a]=true;
		}
		vi V;
		RE(i,N)
			V.push_back(i);
		vi Erg(N,INF);
		do{
			stack<int> s;
			bool ok=true;
			RE(i,V.size()){
				while(s.size()&&!G[V[i]][s.top()])
					s.pop();
				if(i&&!s.size()){
					ok=false;
					break;
				}
				s.push(V[i]);
			}
			if(ok){
				vi W;
				RE(i,N)
					W.push_back(A[V[i]]);
				if(W<Erg)
					Erg=W;
			}
			
		}while(next_permutation(V.begin(),V.end()));
		
		RE(i,Erg.size())
			printf("%d",Erg[i]);
		printf("\n");
	}
	return 0;
}