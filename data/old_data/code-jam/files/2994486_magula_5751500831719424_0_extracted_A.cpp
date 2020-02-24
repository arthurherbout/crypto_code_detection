#include "stdio.h"
#include "string"
#include "algorithm"
#include "math.h"
#include "vector"
#include "set"
#include "queue"
#include "map"
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

int A[200];
vi B[200];

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		RE(i,200)
			B[i].clear();
		printf("Case #%d: ",tc+1);
		int N;scanf("%d ",&N);
		string E;
		bool ok=true;
		RE(n,N){
			fill_n(A,200,0);
			char c[200];
			scanf("%s ",c);
			string s=c;
			string F;
			F+=s[0];
			int r=0;
			A[0]=1;
			REP(i,1,s.length()){
				if(s[i]!=s[i-1]){
					F+=s[i];
					r++;
					A[r]=1;
				}
				else
					A[r]++;
			}
			if(!n){
				E=F;
			}else{
				if(E!=F)
					ok=false;
			}
			RE(i,r+1)
				B[i].push_back(A[i]);
		}
// 		printf("%s\n",E.c_str());
// 		RE(i,E.length()){
// 			RE(j,N)
// 			printf("%d ",B[i][j]);
// 			printf("\n");
// 		}
		if(!ok)
			printf("Fegla Won\n");
		else{
			LL P=0;
			RE(i,E.length()){
				sort(B[i].begin(),B[i].end());
				int M=B[i][N/2];
				RE(j,N)
					P+=abs(M-B[i][j]);
			}
			printf("%lld\n",P);
		}
	}
	return 0;
}