#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <iostream>
#include <queue>
#include <set>
#include <map>

#define ABS(x) ((x)<0?-(x):(x))
#define SQR(x) ((x)*(x))
#define PB push_back
#define MP make_pair
#define SS stringstream
#define LL long long
#define LD long double
#define SZ(x) int(x.size())
#define ALL(x) x.begin(),x.end()
#define FOR(a,b) for(int a=0;a<b;a++)
#define DFOR(a,b) for(int a=b-1;a>=0;a--)
#define CLR(a,b) memset(a,b,sizeof(a))

using namespace std;

int n, m;
char a[55][55];

int dp[16][16][1<<15];

inline void update(int &a, int b) {
	if (b > a) a = b;
}

void solvecase() {
	scanf("%d%d", &n, &m);
	FOR(i, n) scanf("%s", a[i]);
	CLR(dp, -1);
	dp[0][0][0] = 0;
	FOR(i, n) FOR(j, m) FOR(mask, (1<<m)) if (dp[i][j][mask] != -1) {
		int ni = i, nj = j + 1;
		int cur = dp[i][j][mask];
		if (nj == m) {
			ni++;
			nj = 0;
		}
		if (a[i][j] == '#' || a[i][j] == '?') {
			int t = 4;
			if (j > 0 && (mask & (1<<(j-1))) > 0) t-=2;
			if (mask & (1<<j)) t-=2;
			update(dp[ni][nj][mask | (1<<j)], cur + t);
		}
		if (a[i][j] == '.' || a[i][j] == '?') {
			update(dp[ni][nj][mask - (mask & (1<<j))], cur);
		} 
	}
	int res = 0;
	FOR(i, (1<<m)) res = max(res, dp[n][0][i]);
	printf("%d", res);
}

void solve() {
	int n;
	scanf("%d\n", &n);
	FOR(i, n) {
		printf("Case #%d: ", i+1);
		solvecase();
		printf("\n");
	}
}

int main() {
	freopen("x", "rt", stdin);
	//freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);
	solve();
	return 0;
}