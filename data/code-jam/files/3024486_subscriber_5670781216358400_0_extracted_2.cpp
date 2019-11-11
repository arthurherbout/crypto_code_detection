#include<stdio.h>
#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<memory.h>
#include<map>
#include<set>
#include<queue>
#include<list>
#include<sstream>
#define mp make_pair
#define pb push_back      
#define F first
#define S second
#define SS stringstream
#define sqr(x) ((x)*(x))
#define m0(x) memset(x,0,sizeof(x))
#define m1(x) memset(x,63,sizeof(x))
#define CC(x) cout << (x) << endl
#define pw(x) (1ll<<(x))
#define M 1000000007
#define N 111111
using namespace std;
typedef pair<int,int> pt;

int n, c[88];
vector<pt> v[88];
int uv[88], ud[88];
map<long long , int> W;
long long mod = (long long) 1e18 + 7;

int go(int x, int y, int pl) {
	int canx = 0, cany = 0;
	long long hash = x + n * y + n * n * pl;

	for (int i = 0; i < v[x].size(); i++) if (!ud[v[x][i].S]) canx = 1; else hash = hash * (n + 2) + v[x][i].F;
	for (int i = 0; i < v[y].size(); i++) if (!ud[v[y][i].S]) cany = 1; else hash = hash * (n + 3) + v[y][i].F;
	if (canx == 0 && cany == 0 && uv[x] == 1 && uv[y] == 1) return 0;
	hash = hash * 2 + uv[x];
	hash = hash * 2 + uv[y];

//	for (int i = 0; i < n; i++) hash = (hash * 2 + uv[i]) % mod;
//	for (int i = 0; i < n; i++) hash = (hash * 2 + ud[i]) % mod;
	if (W.find(hash) != W.end()) return W[hash];
	if (pl == 0) {
		int g = 0;
		if (!uv[x]) g = c[x];
		int best = -1e9;

		int uu = uv[x];
		if (!canx) {
			uv[x] = 1;
			best = g + go(x, y, 1);
			uv[x] = uu;
		} else {			
			for (int i = 0; i < v[x].size(); i++) if (!ud[v[x][i].S]) {
				uv[x] = 1;
				ud[v[x][i].S] = 1;
				best = max(best, g + go(v[x][i].F, y, 1));		
				uv[x] = uu;
				ud[v[x][i].S] = 0;
			}
		}			
		return W[hash] = best;
	} else {
		int g = 0;
		if (!uv[y]) g = -c[y];
		int wr = 1e9;

		int uu = uv[y];
		if (!cany) {
			uv[y] = 1;
			wr = g + go(x, y, 0);
			uv[y] = uu;
		} else {
			for (int i = 0; i < v[y].size(); i++) if (!ud[v[y][i].S]) {
				uv[y] = 1;
				ud[v[y][i].S] = 1;
				wr = min(wr, g + go(x, v[y][i].F, 0));
				uv[y] = uu;
				ud[v[y][i].S] = 0;
			}
		}			
		return W[hash] = wr;
	}
}

int main(){
	freopen("1.in","r",stdin);	
	freopen("1.out","w",stdout);
	int T;
	cin >> T;
	for (int tt = 1; tt <= T; tt++) {
		cin >> n;
		for (int i = 0; i < n; i++) cin >> c[i];
		for (int i = 0; i < n; i++) v[i].clear();
		for (int i = 0; i < n - 1; i++) {
			int x;
			cin >> x;
			v[i].pb(mp(x - 1, i));
			v[x - 1].pb(mp(i, i));
		}
		W.clear();
		int best = -1e9;
		for (int x = 0; x < n; x++) {
			int wr = 1e9;
			for (int y = 0; y < n; y++) {
				for (int i = 0; i < n; i++) uv[i] = ud[i] = 0;
				int v = go(x, y, 0);
				wr = min(wr, v);
			}
			best = max(best, wr);
		}
		cout << "Case #" << tt << ": ";
		cout << best << endl;

	}
	return 0;
}