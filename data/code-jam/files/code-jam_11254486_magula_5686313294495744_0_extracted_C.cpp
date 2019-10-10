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
template<class t> void setmin(t &a, t b){a=min(a,b);}

int M[2];
map<string,int> H[2];

int S2I(int i, string s){
	if(H[i].find(s)==H[i].end())
		H[i][s]=M[i]++;
	return H[i][s];
}

int A[2000],B[2000];
vi V[2000],W[2000];
bool nonfake[2000];
int Erg;
int N;

void dfs2(int i){
	if(i==M[1]){
		int O=0;
		RE(k,N){
			if(!nonfake[k]&&V[A[k]].size()>1&&W[B[k]].size()>1)
				O++;
		}
		Erg=max(O,Erg);
		return;
	}
	bool ok =false;
	RE(j,W[i].size())
		if(nonfake[W[i][j]])
			ok =true;
	if(ok)
		dfs2(i+1);
	else
		RE(j,W[i].size()){
			nonfake[W[i][j]]=true;
			dfs2(i+1);
			nonfake[W[i][j]]=false;
		}
}

void dfs(int i){
	if(i==M[0]){
		dfs2(0);
		return;
	}
	RE(j,V[i].size()){
		nonfake[V[i][j]]=true;
		dfs(i+1);
		nonfake[V[i][j]]=false;
	}
}

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		
		RE(i,2){
			M[i]=0;
			H[i].clear();
		}
		fill_n(A,2000,0);
		fill_n(B,2000,0);
		RE(j,2000){
			V[j].clear();W[j].clear();
			nonfake[j]=false;
		}
		Erg=0;
		
		scanf("%d ",&N);
		RE(i,N){
			char c[1000],d[1000];
			scanf("%s %s ",c,d);
			A[i]=S2I(0,c);
			B[i]=S2I(1,d);
			V[S2I(0,c)].push_back(i);
			W[S2I(1,d)].push_back(i);
		}
		
		dfs(0);
		
		printf("%d\n",Erg);
	}
	return 0;
}
