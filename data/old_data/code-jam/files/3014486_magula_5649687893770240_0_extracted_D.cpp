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

string A[1005];int M,N;
vector<string> X[105];

int Erg,ways;

void visit(int i){
	if(i==M){
		int P=0;
		RE(n,N){
			P+=(X[n].size()>0);
			set<string> S;
			RE(a,X[n].size()){
				string st;
				RE(b,X[n][a].length()){
					st+=X[n][a][b];
					S.insert(st);
				}
			}
			P+=S.size();
		}
		if(P>Erg){
			Erg=P;
			ways=0;
		}
		if(P==Erg)
			ways++;
		return;
	}
	RE(n,N){
		X[n].push_back(A[i]);
		visit(i+1);
		X[n].pop_back();
	}
}

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		scanf("%d %d ",&M,&N);
		Erg=ways=0;
		RE(m,M){
			char c[120];scanf("%s ",c);
			A[m]=c;
		}
		sort(A,A+M);
		visit(0);
		printf("%d %d\n",Erg,ways);
	}
	return 0; 
}