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

int T;
int N, L;
char a[110][110], b[110];

int main(){
#ifndef ONLINE_JUDGE
	freopen("d.in", "r", stdin);
	freopen("d.out", "w", stdout);
#endif
	scanf("%d", &T);
	for(int t = 1; t <= T; ++t){
		printf("Case #%d: ", t);
		scanf("%d%d", &N, &L);
		bool ok = 1;
		for(int i = 1; i <= N; ++i){
			scanf("%s", a[i]);
			bool flag = 0;
			for(int j = 0; j < L; ++j)
				if(a[i][j] == '0') flag = 1;
			if(!flag) ok = 0;
		}
		scanf("%s", b);
		if(!ok) { printf("IMPOSSIBLE\n"); continue; }
		if(L == 1){
			printf("? 0");
			printf("\n");
			continue;
		}
		for(int i = 0; i < L / 2; ++i) printf("10");
		printf("?");
		printf("1 ");
		for(int i = 0; i < L - 1; ++i) printf("?");
		printf("\n");
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
