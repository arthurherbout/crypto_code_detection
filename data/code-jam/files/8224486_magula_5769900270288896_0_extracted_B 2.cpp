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

bool A[100][100];

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		int R,C,N;scanf("%d %d %d ",&R,&C,&N);
		int Erg=INF;
		RE(bi,1<<(R*C)){
			int H=0;
			RE(r,R)
			RE(c,C)
				H+=(A[r][c]=bi&(1<<(r*C+c)));
			if(H!=N)
				continue;
			int V=0;
			RE(r,R)
			RE(c,C){
// 				printf("%d-%d\n",A[r][c],A[r+1][c]);
				if(r<R-1)	V+=A[r][c]&&A[r+1][c];
				if(c<C-1)	V+=A[r][c]&&A[r][c+1];
			}
			Erg=min(Erg,V);
		}
		printf("%d\n",Erg);
	}
	return 0;
}