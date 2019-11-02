// jam.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>
#include <utility>
#include <cctype>
#include <vector>

using namespace std;

char g[20][20], cc[20][20];

int n, m;
int ox, oy;
int ff[20][20];
int d[8][2] = {{-1,0},{-1,1},{-1,-1},{0,1},{0,-1},{1,0},{1,-1},{1,1}};

int getcc(int x,int y) {
	if (x >= n || y >= m || x < 0 || y < 0) return -1;
	return g[x][y];
}

void dfs(int x, int y) {
	if (ff[x][y]) return;
	ff[x][y] = 1;
	int i, tx, ty;
	for (i = 0; i < 8; i++) {
		tx = x + d[i][0];
		ty = y + d[i][1];
		if (getcc(tx, ty) == '.') dfs(tx, ty);
	}
}

int a[16][16][2][1<<16];

int dp(int row, int pos, int below, int mask) {
	if (row == n) return 0;
	if (pos == m) return dp(row + 1, 0, 0, mask);
	int &ret = a[row][pos][below][mask];
	if (ret >= 0) return ret;
	ret = 0;
	if (g[row][pos] == '#' || (mask & (1 << pos))) {
		mask &= (~ (1 << pos));
		if (below) mask |= (1 << pos);
		ret = dp(row, pos + 1, 0, mask);
		return ret;
	}
	else {
		ret = dp(row, pos + 1, 0, mask | (below * (1 << pos)));
		if (row < n - 1 && g[row + 1][pos] == '.' && !below) {
			int tmp = 1 + dp(row, pos + 1, 0, mask | (1 << pos));
			if (tmp > ret) ret = tmp;
		}
		if (pos < m - 1 && g[row][pos + 1] == '.' && !(mask & (1 << (pos + 1)))) {
			int tmp = 1 + dp(row, pos + 2, 0, mask | (below * (1 << pos)));
			if (tmp > ret) ret = tmp;
		}
		if (pos > 0 && row < n - 1 && g[row + 1][pos - 1] == '.' && !(mask & (1 << (pos - 1)))) {
			int tmp = 1 + dp(row, pos + 1, 0, mask | (1 << (pos - 1)) | (below * (1 << pos)));
			if (tmp > ret) ret = tmp;
		}
		if (pos < m - 1 && row < n - 1 && g[row + 1][pos + 1] == '.' && !(mask & (1 << (pos + 1)))) {
			int tmp = 1 + dp(row, pos + 1, 1, mask | (below * (1 << pos)));
			if (tmp > ret) ret = tmp;
		}
	}
	return ret;
}

int solve() {
	int i, j, k, tx, ty, t1, t2;
	memset(a, 0xff, sizeof(a));
	g[ox][oy] = '.';
	t1 = dp(0, 0, 0, 0);
	memset(a, 0xff, sizeof(a));
	g[ox][oy] = '#';
	t2 = dp(0, 0, 0, 0);
	if (t1 != t2) return 1;
	return 0;
	/*
	for (k = 0; k < 8; k++) {
		memcpy(g, cc, sizeof(cc));
		tx = ox + d[k][0];
		ty = oy + d[k][1];
		if (getcc(tx, ty) == '.') {
			g[ox][oy] = '.';
			memset(ff, 0, sizeof(ff));
			dfs(ox,oy);
			for (i = 0; i < n; i++) {
				for (j = 0; j < n; j++) {
					if (!ff[i][j]) g[i][j] = '#';
				}
			}
			memset(a, 0xff, sizeof(a));
			if (dp(0, 0, 0, 0)) return 1;
			return 0;
		}
	}
	*/
	return 1;
}

int ddfs(int x,int y) {
	int i, tx, ty;
	if (g[x][y] == '.') g[x][y] = '#';
	for (i = 0; i < 8; i++) {
		tx = x + d[i][0];
		ty = y + d[i][1];
		if (tx < 0 || ty < 0 || tx >= n || ty >= m || g[tx][ty] != '.') continue;
		if (!ddfs(tx,ty)) return 1;
	}
	if (g[x][y] == '#') g[x][y] = '.';
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	int zz, testcases;
	int i, j;

	freopen("p43.in","r",stdin);
	freopen("p4.out","w",stdout);

	scanf("%d", &testcases);
	for (zz = 1; zz <= testcases; zz++) {
		printf("Case #%d: ",zz);
		scanf("%d%d",&n,&m);
		for (i = 0; i < n; i++) scanf("%s",g[i]);
		memcpy(cc, g, sizeof(g));
		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++) {
				if (g[i][j] == 'K') {
					ox = i;
					oy = j;
				}
			}
		}
		
		/*
		if (ddfs(ox, oy)) {
			printf("A\n");
		}
		else {
			printf("B\n");
		}
		*/
		if (solve()) {
			printf("A\n");
		}
		else {
			printf("B\n");
		}
	}

	return 0;
}
