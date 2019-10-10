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

int W,H,B;
bool A[105][505];

int Erg;

ii D[]={MP(0,1),MP(1,0),MP(0,-1),MP(-1,0)};

bool visit(int x, int y, int d){
	if(x<0||x>=W||y<0||y>=H)
		return false;
	if(A[x][y])
		return false;
	A[x][y]=true;
	if(y==H-1){
		Erg++;
		return true;
	}
	RE(i,4){
		int x2=x+D[(i+d+3)%4].FI,y2=y+D[(i+d+3)%4].SE;
		if(visit(x2,y2,(i+d+3)%4))
			return true;
	}
	return false;
}

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		scanf("%d %d %d ",&W,&H,&B);
		RE(x,W)
		RE(y,H)
			A[x][y]=0;
		Erg=0;
		RE(b,B){
			int x1,y1,x2,y2;scanf("%d %d %d %d ",&x1,&y1,&x2,&y2);
			REP(x,x1,x2+1)
			REP(y,y1,y2+1)
				A[x][y]=1;
		}
		RE(x,W)
			visit(x,0,0);
		printf("%d\n",Erg);
	}
	return 0; 
}