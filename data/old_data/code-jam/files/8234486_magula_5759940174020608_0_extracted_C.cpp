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

char c[10000];

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		int N;scanf("%d ",&N);
		bool A[2][3000];
		RE(i,2)fill_n(A[i],3000,false);
		map<string,int> S2I;
		vector<int> B[205];
		int D=0;
		int ind=0;
		RE(n,N){
			while(1){
				char c2;scanf("%s%c ",c,&c2);
// 				printf("%s\n",c);
				string s=c;
				if(S2I.find(s)==S2I.end())
					S2I[s]=ind++;
				int I=S2I[s];
				if(n<2){
					if(!A[n][I]&&A[!n][I])
						D++;
					A[n][I]=true;
				}
				else{
					B[n].push_back(I);
				}
				if(c2=='\n')	break;
			}
		}
// 		printf("+%d\n",D);
		int Erg=INF;
		RE(bi,1<<N){
			bool A2[2][3000];
			RE(i,2)fill_n(A2[i],3000,false);
			int C=0;
			REP(n,2,N){
				FE(s,B[n]){
					bool oh=(bool)(bi&(1<<n));
					if(!A2[oh][*s]&&!A[oh][*s]&&
						(A2[!oh][*s]||A[!oh][*s]))
						C++;
					A2[oh][*s]=true;
				}
			}
			Erg=min(Erg,D+C);
		}
		printf("%d\n",Erg);
	}
	return 0;
}