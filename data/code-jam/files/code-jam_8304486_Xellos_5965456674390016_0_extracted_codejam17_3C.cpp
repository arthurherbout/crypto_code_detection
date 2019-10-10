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
		int N;
		cin >> N;
		int out[2][1000][4];
		int inn[1000]; memset(inn,0,sizeof(inn));
		for(int i =0; i < N; i++) for(int j =0; j < 2; j++) {
			cin >> out[j][i][0] >> out[j][i][1] >> out[j][i][2];
			out[j][i][3] =inn[--out[j][i][0]]++;}

		int ans =OVER9000;
		for(int i =0; i < (1<<N); i++) for(int s =0; s < 2; s++) {
			int akt =0, v =0, d =s;
			vector<int> vis(N,0);
			for(int j =0; j < 2*N; j++) {
				vis[v]++;
				while(akt%24 != out[d][v][1]) akt++;
				akt +=out[d][v][2];
				int di =out[d][v][3];
				v =out[d][v][0];
				d =di^((i>>v)&1);
				if(v == 0) d =1-s;}
			bool ok =true;
			for(int j =0; j < N; j++) if(vis[j] != 2) ok =false;
//			if(v != 0) cerr << i << " " << s << " " << "FAIL\n";
			if(ok) ans =min(ans,akt);}
		cout << ans << "\n";}
	return 0;}

// look at my code
// my code is amazing