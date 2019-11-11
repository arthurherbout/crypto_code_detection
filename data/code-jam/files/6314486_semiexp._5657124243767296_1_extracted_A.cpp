#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <set>
#include <map>
#include <thread>
#include <mutex>

using namespace std;
#define MOD @
#define ADD(X,Y) ((X) = ((X) + (Y)%MOD) % MOD)
typedef long long i64; typedef vector<int> ivec; typedef vector<string> svec;

int nTC, tcid;
int N, D[50505][6];
vector<int> rel;
vector<int> chs;

vector<int> graph[303030];
int to[303030], bak[50505];
bool vis[303030];
int waf[303030], wp;

bool aug(int p)
{
	if (vis[p]) return false;
	vis[p] = true;
	waf[wp++] = p;

	for (int q : graph[p]) {
		if (q == to[p]) continue;
		if (bak[q] == -1) {
			bak[q] = p;
			to[p] = q;
			//printf("%d(%d)->%d\n", p, rel[p], q);
			return true;
		} else {
			if (aug(bak[q])) {
				bak[q] = p;
				to[p] = q;
				//printf("%d(%d)->%d\n", p, rel[p], q);
				return true;
			}
		}
	}
	return false;
}
bool forget(int p)
{
	if (to[p] != -1) {
		//printf("forget %d->%d\n", p, to[p]);
		bak[to[p]] = -1;
		to[p] = -1;
		return true;
	}
	return false;
}

bool check(int L)
{
	for (int i = 0; i < rel.size(); ++i) to[i] = -1;
	for (int i = 0; i < N; ++i) bak[i] = -1;

	int cnt = 0;
	for (int i = 0; i < rel.size(); ++i) {
		fill(vis, vis + rel.size(), false);
		if (i >= L && forget(i - L)) --cnt;
		if (aug(i)) ++cnt;

		//printf("%d %d %d %d\n", L, i, rel[i], cnt);
		if (cnt >= L && chs[i] >= L) return true;
	}
	return false;
}

bool aug2(int p)
{
	wp = 0;
	bool ret = aug(p);
	for (int i = 0; i < wp; ++i) vis[waf[i]] = false;
	return ret;
}

int solve()
{
	for (int i = 0; i < rel.size(); ++i) to[i] = -1;
	for (int i = 0; i < N; ++i) bak[i] = -1;
	int ret = 0;

	int clen = 0;
	for (int i = 0; i < rel.size(); ++i) {
		for (;;) {
			//fill(vis, vis + rel.size(), false);
			if (aug2(i)) {
				++clen;
				break;
			} else {
				if (clen == 0) break;
				forget(i - clen);
				--clen;
			}
		}
		//printf("%d %d\n", i, clen);
		ret = max(ret, min(chs[i], clen));
	}
	return ret;
}

int main()
{
	scanf("%d", &nTC);
	tcid = 0;
	
	for (; tcid++ < nTC; ) {
		scanf("%d", &N);
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < 6; ++j) {
				scanf("%d", &(D[i][j]));
			}
		}
		rel.clear();
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < 6; ++j) {
				rel.push_back(D[i][j]);
			}
		}
		sort(rel.begin(), rel.end());
		rel.erase(unique(rel.begin(), rel.end()), rel.end());

		chs = vector<int>(rel.size(), 0);
		chs[0] = 1;
		for (int i = 1; i < rel.size(); ++i) {
			if (rel[i] != rel[i - 1] + 1) {
				chs[i] = 1;
			} else {
				chs[i] = chs[i - 1] + 1;
			}
		}
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < 6; ++j) {
				int lc = lower_bound(rel.begin(), rel.end(), D[i][j]) - rel.begin();
				graph[lc].push_back(i);
			}
		}
		/*
		int left = 0, right = N;
		while (left < right) {
			int mid = (1 + left + right) / 2;
			if (check(mid)) {
				left = mid;
			} else {
				right = mid - 1;
			}
		}
		*/
		printf("Case #%d: %d\n", tcid, solve());
		for (int i = 0; i < rel.size(); ++i) graph[i].clear();
	}
	
	return 0;
}

