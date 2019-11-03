#include <algorithm>
#include <cstdio>
#include <iostream>
using namespace std;

const int MaxN = 10 + 10;
const int MaxS = 1024 + 100;

int totCase, caseNum;

int n, m;

int f[MaxN][MaxS];
bool visited[MaxN][MaxS];

int g[MaxN];

int state[MaxS], next[MaxS], cnt[MaxS];
int size;

bool isValidState(int x);
int cptNextState(int s);
bool checkState(int s1, int s2);
int work();

int main() {
//	freopen("input", "r", stdin);
//	freopen("output", "w", stdout);

	scanf("%d", &totCase);
	for (caseNum = 1; caseNum <= totCase; ++caseNum) {
		scanf("%d %d", &n, &m);
		for (int i = 0; i < n; ++i) {
			g[i] = 0;
			for (int j = 0; j < m; ++j) {
				char chr;
				scanf(" %c", &chr);
				g[i] <<= 1;
				if (chr == 'x') {
					g[i] += 1;
				}
			}
		}
		g[n] = 0;
		printf("Case #%d: %d\n", caseNum, work());
	}
	return 0;
}

int work() {
	size = 0;
	for (int i = 0; i < (1 << m); ++i) {
		if (isValidState(i)) {
			int tmp = 0;
			for (int j = 0; j < m; ++j) {
				if ((i & (1 << j)) != 0)
					++tmp;
			}
			state[size] = i;
			cnt[size] = tmp;
			next[size] = cptNextState(i);
			++size;
		}
	}

	for (int i = 0; i <= n; ++i)
		for (int j = 0; j < (1 << m); ++j)
			visited[i][j] = false;

	visited[0][g[0]] = true;
	f[0][g[0]] = 0;

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < (1 << m); ++j) {
			if (!visited[i][j])
				continue;
			for (int k = 0; k < size; ++k) {
				if (checkState(j, state[k])) {
					int tmp = (next[k] | g[i + 1]);
					if (!visited[i + 1][tmp]) {
						visited[i + 1][tmp] = true;
						f[i + 1][tmp] = cnt[k] + f[i][j];
					} else
						f[i + 1][tmp] = max(f[i + 1][tmp], cnt[k] + f[i][j]);
				}
			}
		}
	int res = 0;
	for (int i = 0; i < (1 << m); ++i)
		if (visited[n][i] && f[n][i] > res)
			res = f[n][i];
	return res;
}

bool checkState(int s1, int s2) {
	for (int i = 0; i < m; ++i) {
		if ((s1 & (1 << i)) != 0 && (s2 & (1 << i)) != 0)
			return false;
	}
	return true;
}

bool isValidState(int x) {
	for (int i = 0; i < m; ++i) {
		if ((x & (1 << i)) == 0)
			continue;
		if (i < m - 1 && (x & (1 << (i + 1))) != 0)
			return false;
	}
	return true;
}

int cptNextState(int s) {
	int tmp[MaxN];
	fill(tmp, tmp + m, 0);
	for (int i = 0; i < m; ++i) {
		if ((s & (1 << i)) != 0) {
			if (i > 0)
				tmp[i - 1] = 1;
			if (i < m - 1)
				tmp[i + 1] = 1;
		}
	}
	int res = 0;
	for (int i = 0; i < m; ++i) {
		res = (res << 1) + tmp[i];
	}
	return res;
}
