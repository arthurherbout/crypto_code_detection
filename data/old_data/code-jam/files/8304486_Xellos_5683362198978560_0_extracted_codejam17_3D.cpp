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
#define OVER9000 2234567890123456789LL
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
		int R,C,N,D;
		cin >> R >> C >> N >> D;
		cat mod =1000000007;
		vector< vector<cat> > br(R,vector<cat>(C,-1));
		vector< pair<int,int> > init(N);
		for(int i =0; i < N; i++) {
			int x,y,b;
			cin >> x >> y >> b;
			init[i] =make_pair(x-1,y-1);
			br[x-1][y-1] =b;}
		
/*		int dx[] ={1,-1,0,0};
		int dy[] ={0,0,1,-1};
*/
		bool ok =true;
		for(int i =0; i < R; i++) for(int j =0; j < C; j++) {
			if(br[i][j] == -1) {
				br[i][j] =OVER9000;
				ALL_THE(init,it) br[i][j] =min(br[i][j],1LL*(abs(i-it->ff)+abs(j-it->ss))*D+br[it->ff][it->ss]);
				}
			else {
				cat b =br[i][j];
				ALL_THE(init,it) b =min(b,1LL*(abs(i-it->ff)+abs(j-it->ss))*D+br[it->ff][it->ss]);
				if(b != br[i][j]) ok =false;}
			}
		if(!ok) {cout << "IMPOSSIBLE\n"; continue;}
		cat ans =0;
		for(int i =0; i < R; i++) for(int j =0; j < C; j++) {
//			cout << br[i][j] << ((j == C-1)?"\n":" ");
			ans =(ans+br[i][j])%mod;}
		if(ans < 0) ans +=mod;
		cout << ans << "\n";}
	return 0;}

// look at my code
// my code is amazing