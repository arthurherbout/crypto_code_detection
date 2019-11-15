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

int T, n, m, b[20][20], x, y, a[40];

int walk(int d, int i, int j){
	if(i < 1) return j;
	if(j > m) return n + i;
	if(i > n) return n + m + m - j + 1;
	if(j < 1) return n + m + m + n - i + 1;
	if(d == 0){
		if(b[i][j]) return walk(1, i + 1, j);
		else return walk(3, i - 1, j);
	}else if(d == 1){
		if(b[i][j]) return walk(0, i, j + 1);
		else return walk(2, i, j - 1);
	}else if(d == 2){
		if(b[i][j]) return walk(3, i - 1, j);
		else return walk(1, i + 1, j);
	}else{
		if(b[i][j]) return walk(2, i, j - 1);
		else return walk(0, i, j + 1);
	}
}

int main(){
#ifndef ONLINE_JUDGE
	freopen("c.in", "r", stdin);
	freopen("c.out", "w", stdout);
#endif
	scanf("%d", &T);
	for(int t = 1; t <= T; ++t){
		scanf("%d%d", &n, &m);
		for(int i = 1; i <= (n + m); ++i){
			scanf("%d%d", &x, &y);
			a[x] = y, a[y] = x;
		}
		bool ck = 0;
		printf("Case #%d:\n", t);
		for(int mask = 0; mask < (1 << (n * m)); ++mask){
			for(int i = 1; i <= n; ++i)
				for(int j = 1; j <= m; ++j)
					b[i][j] = (mask >> ((i - 1) * m + j - 1)) & 1;
			
			bool flag = 1;
			for(int i = 1; i <= m; ++i)
				if(walk(1, 1, i) != a[i]) { flag = 0; break; }
			if(!flag) continue;
			for(int i = 1; i <= n; ++i)
				if(walk(2, i, m) != a[i + m]) { flag = 0; break; }
			if(!flag) continue;
			for(int i = 1; i <= m; ++i)
				if(walk(3, n, i) != a[m - i + 1 + n + m]) { flag = 0; break; }
			if(!flag) continue;
			for(int i = 1; i <= n; ++i)
				if(walk(0, i, 1) != a[n - i + 1 + n + m + m]) { flag = 0; break; }
			if(!flag) continue;
			ck = 1;
			for(int i = 1; i <= n; ++i, printf("\n"))
				for(int j = 1; j <= m; ++j)
					if(b[i][j]) printf("\\"); else printf("/");
			break;
		}
		if(!ck) printf("IMPOSSIBLE\n");
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
