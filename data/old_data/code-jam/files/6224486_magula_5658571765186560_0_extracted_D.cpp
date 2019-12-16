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

#define INF 1000000000000000000
#define MP make_pair
#define FI first
#define SE second

#define Result(a) {printf("Case #%d: %s\n",tc+1,a?"RICHARD":"GABRIEL");continue;}

int X,R,C;
bool A[100][100],vis[100][100];

ii D[]={MP(-1,0),MP(0,1),MP(0,-1),MP(1,0)};

int dfs(ii a){
	vis[a.FI][a.SE]=true;
	int E=0;
	RE(d,4){
		ii b=MP(a.FI+D[d].FI,a.SE+D[d].SE);
		if(b.FI>=0&&b.SE>=0&&b.FI<R&&b.SE<C&&!vis[b.FI][b.SE]){
			E+=dfs(b);
		}
	}
	return E+1;
}

vi V;
map<vi,bool> H;
int Ra;

void visit(int k, ii a){
	A[a.FI][a.SE]=true;
	if(k==X){
		RE(i,R)RE(j,C)	vis[i][j]=A[i][j];
		bool ok=true;
		RE(i,R)
		RE(j,C)
			if(!vis[i][j])
				if(dfs(MP(i,j))%X!=0){
					ok=false;
				}
		RE(l,4){
			RE(g,V.size())
				V[g]=(V[g]+1)%4;
			if(H.find(V)==H.end()){
				H[V]=false;
				Ra++;
			}
			if(!H[V]&&ok)
				Ra--;
			H[V]|=ok;
		}
		
	}else{
		RE(d,4){
			ii b=MP(a.FI+D[d].FI,a.SE+D[d].SE);
			if(b.FI>=0&&b.SE>=0&&b.FI<R&&b.SE<C&&!A[b.FI][b.SE]){
				V.push_back(d);
				visit(k+1,b);
				V.pop_back();
			}
		}
	}
	A[a.FI][a.SE]=false;
}

vii Y[5];

int main(){
	Y[1].push_back(MP(1,1));
	Y[2].push_back(MP(1,2));
	Y[3].push_back(MP(1,3));
	Y[3].push_back(MP(2,2));
	Y[4].push_back(MP(1,4));
	Y[4].push_back(MP(2,2));
	Y[4].push_back(MP(2,3));
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		RE(i,4)RE(j,4)	A[i][j]=false;
		scanf("%d %d %d ",&X,&R,&C);
		ii S=MP(min(R,C),max(R,C));
		bool o=false;
		FE(x,Y[X])
			if(x->FI>S.FI||x->SE>S.SE)
				o=true;
		if(o)	Result(1);
		ii a;
		Ra=false;
		H.clear();
		for(a.FI=0;a.FI<R;a.FI++)
		for(a.SE=0;a.SE<C;a.SE++)
			visit(1,a);
		Result(Ra!=0);
	}
	return 0;
}