#include <bits/stdc++.h>
// iostream is too mainstream
#include <cstdio>
// bitch please
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <list>
#include <cmath>
#include <iomanip>
#include <time.h>
#define dibs reserve
#define OVER9000 1234567890
#define ALL_THE(CAKE,LIE) for(auto LIE =CAKE.begin(); LIE != CAKE.end(); LIE++)
#define tisic 47
#define soclose 1e-8
#define chocolate win
// so much chocolate
#define patkan 9
#define ff first
#define ss second
#define abs(x) ((x < 0)?-(x):x)
#define uint unsigned int
#define dbl long double
#define pi 3.14159265358979323846
using namespace std;
// mylittledoge

#ifdef DONLINE_JUDGE
	// palindromic tree is better than splay tree!
	#define lld I64d
#endif

int main() {
	cin.sync_with_stdio(0);
	cin.tie(0);
	cout << fixed << setprecision(10);
	int T;
	cin >> T;
	for(int t =0; t < T; t++) {
		cout << "Case #" << t+1 << ":\n";
		int R,C;
		cin >> R >> C;
		vector< pair<int,int> > L(R+C);
		vector< vector<int> > pos((R+2),vector<int>(C+2,-1));
		for(int i =1; i <= C; i++) pos[0][i] =i;
		for(int i =1; i <= R; i++) pos[i][C+1] =i+C;
		for(int i =C; i >= 1; i--) pos[R+1][i] =(C+1-i)+R+C;
		for(int i =R; i >= 1; i--) pos[i][0] =(R+1-i)+R+C+C;
		for(int i =0; i < R+C; i++) cin >> L[i].ff >> L[i].ss;
		vector<string> ans;
		for(int i =0; i < (1<<(R*C)); i++) {
			vector< vector<int> > G(4*(R+2)*(C+2));
			vector<string> akt(R);
			for(int i =1; i <= C; i++) for(int j =0; j <= R; j++) {
				G[(j*(C+2)+i)*4+2].push_back(((j+1)*(C+2)+i)*4+0);
				G[((j+1)*(C+2)+i)*4+0].push_back((j*(C+2)+i)*4+2);}
			for(int i =0; i <= C; i++) for(int j =1; j <= R; j++) {
				G[(j*(C+2)+i)*4+1].push_back((j*(C+2)+i+1)*4+3);
				G[(j*(C+2)+i+1)*4+3].push_back((j*(C+2)+i)*4+1);}
			for(int j =0; j < R; j++) for(int k =0; k < C; k++) {
				int x =j*C+k;
				if((i>>x)&1) {
					akt[j] +="\\";
					G[(j*(C+2)+(k+1))*4+2].push_back(((j+1)*(C+2)+(k+2))*4+3);
					G[((j+2)*(C+2)+(k+1))*4+0].push_back(((j+1)*(C+2)+(k))*4+1);
					G[((j+1)*(C+2)+(k+2))*4+3].push_back((j*(C+2)+(k+1))*4+2);
					G[((j+1)*(C+2)+(k))*4+1].push_back(((j+2)*(C+2)+(k+1))*4+0);}
				else {
					akt[j] +="/";
					G[(j*(C+2)+(k+1))*4+2].push_back(((j+1)*(C+2)+(k))*4+1);
					G[((j+2)*(C+2)+(k+1))*4+0].push_back(((j+1)*(C+2)+(k+2))*4+3);
					G[((j+1)*(C+2)+(k))*4+1].push_back((j*(C+2)+(k+1))*4+2);
					G[((j+1)*(C+2)+(k+2))*4+3].push_back(((j+2)*(C+2)+(k+1))*4+0);}
				}
			queue<int> q;
			vector<bool> vis((R+2)*(C+2)*4,false);
			bool ok =true;
			for(int a =0; a <= R+1; a++) for(int b =0; b <= C+1; b++) if(pos[a][b] != -1) {
				if(a == 0) q.push((a*(C+2)+b)*4+2);
				if(a == R+1) q.push((a*(C+2)+b)*4+0);
				if(b == 0) q.push((a*(C+2)+b)*4+1);
				if(b == C+1) q.push((a*(C+2)+b)*4+3);
				if(vis[q.front()]) {
					q.pop();
					continue;}
				vis[q.front()] =true;
				vector<int> conn;
				while(!q.empty()) {
					int y =q.front()/4;
					if(pos[y/(C+2)][y%(C+2)] != -1)
						conn.push_back(pos[y/(C+2)][y%(C+2)]);
					ALL_THE(G[q.front()],it) if(!vis[*it]) {
						int z =(*it)/4;
						vis[*it] =true;
						q.push(*it);}
					q.pop();}
				if((int)conn.size() != 2) ok =false;
				for(int j =0; j < R+C; j++) if(L[j].ff == pos[a][b] || L[j].ss == pos[a][b])
					if(L[j].ff != conn[1] && L[j].ss != conn[1]) ok =false;
				if(!ok) break;}
			if(!ok) continue;
			ans =akt;
			break;}
		if(ans.empty()) cout << "IMPOSSIBLE\n";
		else for(int i =0; i < R; i++) cout << ans[i] << "\n";
		}
	return 0;}

// look at my code
// my code is amazing
