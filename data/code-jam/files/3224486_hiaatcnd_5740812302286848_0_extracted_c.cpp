//看看会不会爆int!
#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define pb push_back
#define mkp make_pair
#define fi first
#define se second
#define FOR(i, l, r) for(int i = l; i <= r; i++)
#define ROF(i, r, l) for(int i = r; i >= l; i--)
#define all(a) a.begin(), a.end()
inline int ckmax(int &a, int b) { return a < b ? a = b, 1 : 0; }
inline int ckmin(int &a, int b) { return a > b ? a = b, 1 : 0; }

const int maxn = 1010;
int T, n, s;
int x[maxn], y[maxn], z[maxn], vx[maxn], vy[maxn], vz[maxn];
int d[maxn][maxn], fa[maxn];
bool vis[maxn];

int sqr(int x) { return x * x; }
int find(int x) { return fa[x] == x ? x : fa[x] = find(fa[x]); }
void merge(int x, int y){
	int xx = find(x), yy = find(y);
	fa[yy] = xx;
}

int main(){
#ifndef ONLINE_JUDGE
	freopen("c.in", "r", stdin);
	freopen("c.out", "w", stdout);
#endif
	scanf("%d", &T);
	for(int t = 1; t <= T; ++t){
		scanf("%d%d", &n, &s);
		for(int i = 1; i <= n; ++i){
			scanf("%d%d%d%d%d%d", x + i, y + i, z + i, vx + i, vy + i, vz + i);
		}
		vector<pair<int, pair<int, int> > > q;
		for(int i = 1; i <= n; ++i){
			for(int j = 1; j < i; ++j){
				d[i][j] = sqr(x[i] - x[j]) + sqr(y[i] - y[j]) + sqr(z[i] - z[j]);
				q.pb(mkp(d[i][j], mkp(i, j)));
			}
		}
		for(int i = 1; i <= n; ++i) fa[i] = i;
		sort(all(q)); int last = 0;
		for(auto v : q){
			if(find(1) == find(2)) break;
			merge(v.se.se, v.se.fi);
			last = v.fi;
		}
		printf("Case #%d: %.7f\n", t, sqrt(last));
	}
  return 0;
}
/*
                   _ooOoo_
                  o8888888o
                  88" . "88
                  (| -_- |)
                  O\  =  /O
               ____/`---'\____
             .'  \\|     |//  `.
            /  \\|||  :  |||//  \
           /  _||||| -:- |||||-  \
           |   | \\\  -  /// |   |
           | \_|  ''\---/''  |   |
           \  .-\__  `-`  ___/-. /
         ___`. .'  /--.--\  `. . __
      ."" '<  `.___\_<|>_/___.'  >'"".
     | | :  `- \`.;`\ _ /`;.`/ - ` : | |
     \  \ `-.   \_ __\ /__ _/   .-` /  /
======`-.____`-.___\_____/___.-`____.-'======
                   `=---='
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
         佛祖保佑       永无BUG
*/
