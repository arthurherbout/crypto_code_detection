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

typedef long long cat;

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
		if(T >= 10) {cerr << t << ((t == T-1)?"\n":" "); flush(cerr);}
		cout << "Case #" << t+1 << ": ";
		int C,R,M;
		cin >> C >> R >> M;
		vector<string> V(R);
		for(int i =0; i < R; i++) cin >> V[i];
		vector< pair<int,int> > S;
		vector< pair<int,int> > tur;
		for(int i =0; i < R; i++) for(int j =0; j < C; j++) {
			if(V[i][j] == 'S') S.push_back(make_pair(i,j));
			if(V[i][j] == 'T') tur.push_back(make_pair(i,j));}
		int ns =S.size(), nt =tur.size();
		int dx[] ={-1,1,0,0};
		int dy[] ={0,0,-1,1};

		vector< vector< vector<int> > > LOS(nt,vector< vector<int> >(R,vector<int>(C,0)));
		for(int j =0; j < nt; j++) {
			for(int k =tur[j].ff; k < R; k++) {
				if(V[k][tur[j].ss] == '#') break;
				LOS[j][k][tur[j].ss] =1;}
			for(int k =tur[j].ff; k >= 0; k--) {
				if(V[k][tur[j].ss] == '#') break;
				LOS[j][k][tur[j].ss] =1;}
			for(int k =tur[j].ss; k < C; k++) {
				if(V[tur[j].ff][k] == '#') break;
				LOS[j][tur[j].ff][k] =1;}
			for(int k =tur[j].ss; k >= 0; k--) {
				if(V[tur[j].ff][k] == '#') break;
				LOS[j][tur[j].ff][k] =1;}
			}

		vector< vector< vector<int> > > dst[1<<10];
		vector< vector< vector<int> > > kill(1<<nt,vector< vector<int> >(ns,vector<int>(nt,0)));
		for(int i =0; i < (1<<nt); i++) {
			dst[i].resize(ns,vector< vector<int> >(R,vector<int>(C,R*C+10)));
			vector< vector<int> > LOSc(R,vector<int>(C,0));
			for(int j =0; j < nt; j++) if((i>>j)&1)
				for(int k =0; k < R*C; k++) LOSc[k/C][k%C] |=LOS[j][k/C][k%C];
			for(int j =0; j < ns; j++) {
				queue< pair<int,int> > q;
				q.push(S[j]);
				dst[i][j][S[j].ff][S[j].ss] =0;
				while(!q.empty()) {
					int x =q.front().ff, y =q.front().ss;
					if(LOSc[x][y]) {q.pop(); continue;}
					for(int k =0; k < 4; k++) {
						int x2 =max(0,min(R-1,x+dx[k])), y2 =max(0,min(C-1,y+dy[k]));
						if(V[x2][y2] == '#' || dst[i][j][x2][y2] <= dst[i][j][x][y]+1) continue;
						dst[i][j][x2][y2] =dst[i][j][x][y]+1;
						q.push(make_pair(x2,y2));}
					q.pop();}
				for(int k =0; k < nt; k++) if((i>>k)&1)
					for(int x =0; x < R; x++) if(kill[i][j][k] == 0) for(int y =0; y < C; y++) 
						if(dst[i][j][x][y] <= M && LOS[k][x][y]) {kill[i][j][k] =1; break;}
				}
			}

		vector< vector<int> > stat(1<<nt,vector<int>(1<<ns,0));
		stat[(1<<nt)-1][(1<<ns)-1] =1;
		for(int i =(1<<nt)-1; i > 0; i--) for(int j =0; j < (1<<ns); j++) if(stat[i][j])
			for(int k =0; k < ns; k++) if((j>>k)&1) for(int l =0; l < nt; l++) if((i>>l)&1)
				if(kill[i][k][l]) stat[i^(1<<l)][j^(1<<k)] =1;
		int ans =0;
		for(int i =(1<<nt)-1; i >= 0; i--) for(int j =0; j < (1<<ns); j++) if(stat[i][j]) {
			int akt =nt;
			for(int k =0; k < nt; k++) if((i>>k)&1) akt--;
			ans =max(ans,akt);}
		cout << ans << "\n";
		bool done =false;
		for(int i =(1<<nt)-1; i >= 0; i--) for(int j =0; j < (1<<ns); j++) if(stat[i][j]) {
			if(done) break;
			int akt =nt;
			for(int k =0; k < nt; k++) if((i>>k)&1) akt--;
			if(ans != akt) continue;
			int curi =i, curj =j;
			vector< pair<int,int> > st;
			while(curi != (1<<nt)-1) {
				for(int k =0; k < ns; k++) if(((curj>>k)&1) == 0) for(int l =0; l < nt; l++) if(((curi>>l)&1) == 0)
					if(stat[curi^(1<<l)][curj^(1<<k)] && kill[curi^(1<<l)][k][l]) {
						curi ^=1<<l;
						curj ^=1<<k;
						st.push_back(make_pair(k+1,l+1));}
				}
			reverse(begin(st),end(st));
			ALL_THE(st,it) cout << it->ff << " " << it->ss << "\n";
			done =true;}
		}
	return 0;}

// look at my code
// my code is amazing