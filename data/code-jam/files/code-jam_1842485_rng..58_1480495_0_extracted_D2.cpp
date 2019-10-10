#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <utility>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>

using namespace std;

#define REP(i,n) for((i)=0;(i)<(int)(n);(i)++)
#define foreach(c,itr) for(__typeof((c).begin()) itr=(c).begin();itr!=(c).end();itr++)

int X,Y;
string board[70];
bool can[70][70];
bool a[70][70][70][70]; // W2, l, r, pos
int b[70][70][70][70];
queue <int> q;

void add(int l, int r, int pos){
	q.push(l); q.push(r); q.push(pos);
}

void bfs(int W, int W2){
	int i,j,k;
	
	REP(i,W+1) REP(j,W+1) REP(k,W2) a[W2][i][j][k] = false;
	
	a[W2][0][W][0] = true;
	add(0,W,0);
	
	while(!q.empty()){
		int l = q.front(); q.pop(); int r = q.front(); q.pop(); int pos = q.front(); q.pop();
		
		// move right
		int l2 = min(l+1,W-1), r2 = min(r+1,W), pos2 = min(pos+1,W2-1);
		if(!a[W2][l2][r2][pos2]){
			a[W2][l2][r2][pos2] = true;
			add(l2,r2,pos2);
		}
		
		// move left
		l2 = max(l-1,0), r2 = max(r-1,1), pos2 = max(pos-1,0);
		if(!a[W2][l2][r2][pos2]){
			a[W2][l2][r2][pos2] = true;
			add(l2,r2,pos2);
		}
	}
	
	REP(i,W+1) REP(j,W+1) REP(k,W2) if(a[W2][i][j][k]) b[W][W2][i][k] = max(b[W][W2][i][k],j);
}

bool candown[70][70];

bool check(int gx, int gyl, int gyr, int sx, int syl, int syr){
	int i,j,k;
	
	int W = gyr - gyl + 1;
	int W2 = syr - syl + 1;
	
	REP(i,W+1) REP(j,W+1) candown[i][j] = false;
	REP(i,W) for(j=i;j<W;j++){
		if(board[gx+1][gyl+j] != '#' && !can[gx+1][gyl+j]) break;
		candown[i][j+1] = true;
	}
	
	REP(k,W2) if(board[sx+1][syl+k] != '#') REP(i,W+1) for(j=i+1;j<=W;j++) if(candown[i][j] && b[W][W2][i][k] >= j) return true;
	return false;
}

void func(char testcase, int gx, int gy){
	int i,j,k,l,p,q;
	
	REP(i,X) REP(j,Y) can[i][j] = false;
	can[gx][gy] = true;
	
	REP(k,X+5){
		REP(i,X) REP(j,Y) if(board[i][j] != '#' && board[i][j+1] != '#' && can[i][j]) can[i][j+1] = true;
		REP(i,X) for(j=Y-1;j>=0;j--) if(board[i][j] != '#' && board[i][j-1] != '#' && can[i][j]) can[i][j-1] = true;
		REP(i,X) REP(j,Y) if(board[i][j] != '#' && board[i-1][j] != '#' && can[i][j]) can[i-1][j] = true;
	}
	
	int cnt = 0;
	REP(i,X) REP(j,Y) if(can[i][j]) cnt++;
	cout << testcase << ": " << cnt << ' ';
	
//	for(i=1;i<=W;i++) for(j=1;j<=W;j++) bfs(i,j);
	
	bool ans = true;
	
	REP(p,X) REP(q,Y) if(can[p][q] && !can[p][q-1]){
		int r = q;
		while(can[p][r+1]) r++;
		
		REP(i,p) REP(j,Y) if(can[i][j] && !can[i][j-1]){
			k = j;
			while(can[i][k+1]) k++;
			if(!check(p, q, r, i, j, k)) ans = false;
		}
	}
	
/*	int gyl = gy, gyr = gy;
	while(board[gx][gyl-1] != '#') gyl--;
	while(board[gx][gyr+1] != '#') gyr++;
	int W = gyr - gyl + 1;
*/	
/*	for(i=1;i<=Y;i++) bfs(W,i);
	
	REP(i,gx) REP(j,Y) if(can[i][j] && !can[i][j-1]){
		k = j;
		while(can[i][k+1]) k++;
		if(!check(gx, gyl, gyr, i, j, k)){
			ans = false;
		//	cout << i << ' ' << j << ' ' << k << endl;
		}
	}
*/	
	cout << (ans ? "Lucky" : "Unlucky") << endl;
}

void main2(void){
	int i,j;
	cin >> X >> Y;
	REP(i,X) cin >> board[i];
	for(char ch='0';ch<='9';ch++) REP(i,X) REP(j,Y) if(board[i][j] == ch) func(ch, i, j);
}

int main(void){
	int T,t,i,j,k,l;
	
	REP(i,62) REP(j,62) REP(k,62) REP(l,62) b[i][j][k][l] = -1;
	for(i=1;i<=60;i++) for(j=1;j<=60;j++) bfs(i,j);
	
	cin >> T;
	REP(t,T){
		printf("Case #%d:\n",t+1);
		main2();
	}
	
	return 0;
}
