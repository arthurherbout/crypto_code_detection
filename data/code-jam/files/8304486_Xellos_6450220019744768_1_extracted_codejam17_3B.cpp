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

void DFS(int R, auto &G, auto &dep, auto &par) {
	ALL_THE(G[R],it) if(par[it->ff].ff == -1) {
		par[it->ff] =make_pair(R,it->ss);
		dep[it->ff] =dep[R]+1;
		DFS(it->ff,G,dep,par);}
	}

int main() {
	cin.sync_with_stdio(0);
	cin.tie(0);
	cout << fixed << setprecision(10);
	int T;
	cin >> T;
	for(int t =0; t < T; t++) {
		if(T >= 10) {cerr << t << ((t == T-1)?"\n":" "); flush(cerr);}
		cout << "Case #" << t+1 << ": ";
		int N,M;
		cin >> N >> M;
		vector< vector< pair<int,int> > > G(N);
		vector< pair<int,int> > E(M);
		vector<int> val(M,0);
		for(int i =0; i < M; i++) {
			int a,b;
			cin >> a >> b;
			E[i] =make_pair(--a,--b);
			G[a].push_back(make_pair(b,i));
			G[b].push_back(make_pair(a,i));}

		vector<int> dep(N);
		vector< pair<int,int> > par(N,make_pair(-1,-1));
		for(int i =0; i < N; i++) if(par[i].ff == -1) {
			par[i].ff =i;
			DFS(i,G,dep,par);}

		for(int i =0; i < M; i++) {
			int x =E[i].ff, y =E[i].ss;
			if(dep[x] > dep[y]) swap(x,y);
			if(par[y].ss == i) continue;
			int akt =y;
			while(akt != x) {
				val[par[akt].ss]++;
				akt =par[akt].ff;}
			val[i] =1;}

		bool ok =true;
		for(int i =0; i < M; i++) {
			if(val[i] == 0) ok =false;
			if(val[i] > N*N) cerr << "FAIL ";}
		if(!ok) {cout << "IMPOSSIBLE\n"; continue;}

		for(int i =0; i < M; i++) {
			int x =E[i].ff, y =E[i].ss;
			if(dep[x] > dep[y]) swap(x,y);
			if(par[y].ss == i) {
				if(x == E[i].ss) val[i] *=-1;
				}
			else {
				if(x == E[i].ff) val[i] *=-1;
				}		
			cout << val[i] << ((i == M-1)?"\n":" ");}
		}
	return 0;}

// look at my code
// my code is amazing