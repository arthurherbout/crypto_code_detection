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

int X[300];

int A[110][110];
bool B[4];

ii D[]={MP(-1,0),MP(0,1),MP(1,0),MP(0,-1)};

int main(){
	X['^']=0;
	X['>']=1;
	X['v']=2;
	X['<']=3;
	X['.']=-1;
	
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		int R,C;scanf("%d %d ",&R,&C);
		RE(r,R)RE(c,C){
			char l;scanf("%c ",&l);
			A[r][c]=X[l];
		}
		bool ok=true;
		int Erg=0;
		RE(r,R)RE(c,C)if(A[r][c]!=-1){
			RE(d,4){
				B[d]=false;
				ii n=MP(r,c);
				while(n.FI>=0&&n.SE>=0&&n.FI<R&&n.SE<C){
					if(n!=MP(r,c))B[d]|=A[n.FI][n.SE]!=-1;
					n=MP(n.FI+D[d].FI,n.SE+D[d].SE);
				}
			}
			int e=A[r][c];
			Erg+=!B[e];
			if(!B[0]&&!B[1]&&!B[2]&&!B[3])
				ok=false;
		}
		if(!ok)	printf("IMPOSSIBLE\n");
		else	printf("%d\n",Erg);
	}
	return 0;
}