#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime> 
#define all(c) (c).begin(),(c).end()
#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define ll long long 
#define sz(a) int((a).size())
#define pb push_back
#define SORT(x) sort((x).begin(),(x).end()); 
#define VI vector<int>
#define VII vector < vector <int> >
#define MP make_pair
#define SET(x,byte) memset(x,byte,sizeof(x));
#define I(x) ((x) - 'A')
#define REP(i,mx) for(int i=0;(i)<(mx);i++)
#define RFOR(i,mx,mn) for(int i=(mx);(i)>=(mn);i--)
#define FORD(i,mn,mx,k) for(int i=(mn);(i)<=(mx);i+=(k))
#define FOR(i,mn,mx) for(int i=mn;(i)<=(mx);i++)
#define U(x) (1<<(x))
#define INF 1000000000
#define DEBUG 0

using namespace std;
int nx,ny;
int ekx[4] = {-1, 0,  0,1};
int eky[4] = {0, -1, 1,0};
int sink[101][101];
char harf[101][101];
int grid[101][101];
int h,w;

	char tmp[101][101];
	char vis[101][101];
int issink(int i,int j){
	REP(k,4){
		nx = ekx[k] + i;
		ny = eky[k] + j;
		if(nx<0 || ny<0 || nx>=h || ny>=w) continue;
		if(grid[nx][ny] < grid[i][j] ) return 0;
	}
	return 1;
}
int mini(int i,int j){
	int minim = INF;
	REP(k,4){
		nx = ekx[k] + i;
		ny = eky[k] + j;
		if(nx<0 || ny<0 || nx>=h || ny>=w) continue;
		minim = min(minim, grid[nx][ny]);
	}
	return minim;
}

char yap(int x,int y){
	if(harf[x][y]) return harf[x][y];
	int m = mini(x,y);
	REP(k,4){
		nx = ekx[k] + x;
		ny = eky[k] + y;
		if(nx<0 || ny<0 || nx>=h || ny>=w) continue;
		if(grid[nx][ny] == m){
			return harf[x][y] = yap(nx,ny);
		}
	}
}
int dfs(int x,int y,char h,char must){
	
	vis[x][y] = 1;
	tmp[x][y] = h;
	REP(k,4){
		nx = ekx[k] + x;
		ny = eky[k] + y;
		if(nx<0 || ny<0 || nx>=h || ny>=w) continue;
		if(!vis[nx][ny] && harf[nx][ny] == must) dfs(nx,ny,h,must);
	}
}
void formreal(){
	SET(vis,0);
	SET(tmp,0);
	char now='a';
	REP(i,h) REP(j,w){
		if(!vis[i][j]) dfs(i,j,now++,harf[i][j]);
	}
}
int main(){
	int t,cs=0;
	scanf("%d",&t);
	while(t--){
		scanf("%d%d",&h,&w);
		REP(i,h) REP(j,w) scanf("%d",&grid[i][j]);
		SET(sink,0);
		SET(harf,0);
		REP(i,h) REP(j,w) if(issink(i,j)) sink[i][j] = 1;
		
		char now = 'a';
		REP(i,h) REP(j,w){
			if(sink[i][j]) harf[i][j] = now++;
		}
		REP(i,h) REP(j,w){
			if(sink[i][j]){
				continue;
			}
			else if(harf[i][j]==0) harf[i][j]=yap(i,j);
		}
		/*
		REP(i,h){ 
			REP(j,w-1) printf("%c ",harf[i][j]);
			printf("%c\n",harf[i][w-1]);
		}
		*/
		
		formreal();
		memcpy(harf,tmp,sizeof(harf));
		cs++;
		printf("Case #%d:\n",cs);
		REP(i,h){ 
			REP(j,w-1) printf("%c ",harf[i][j]);
			printf("%c\n",harf[i][w-1]);
		}
	}
	return 0;
}
