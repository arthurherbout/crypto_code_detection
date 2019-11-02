#include <bits/stdc++.h>

using namespace std;

const int inf = 0x3f3f3f3f;
map<pair<pair<int, int>, pair<int, int> >, int> M;
int H, b, d;
int dfs(int h1, int a1, int h2, int a2) {
	if (h2 <= 0) return 0;
	if (h1 <= 0) return inf;
	pair<pair<int, int>, pair<int, int> > S = make_pair(make_pair(h1, a1), make_pair(h2, a2));
	if (M.count(S)) return M[S];
	M[S] = inf;
	int tmp = inf;
	tmp = dfs(h1 - a2, a1, h2 - a1, a2);
	if (a1 < h2) {
		tmp = min(tmp, dfs(h1 - a2, a1 + b, h2, a2));
		tmp = min(tmp, dfs(H - a2, a1, h2, a2));
		int ntk = max(0, a2 - d);
		tmp = min(tmp, dfs(h1 - ntk, a1, h2, ntk));
	}
	tmp = min(tmp, inf - 1);
	return M[S] = tmp + 1;
}

int main( ) {
	int T, tp = 0, a1, h2, a2;
	scanf("%d", &T);
	while (T --) {
		scanf("%d %d %d %d %d %d", &H, &a1, &h2, &a2, &b, &d);
		M.clear();
		int tmp = dfs(H, a1, h2, a2);
		printf("Case #%d: ", ++ tp);
		if (tmp == inf) printf("IMPOSSIBLE\n");
		else printf("%d\n", tmp);
	}
	return 0;
}
