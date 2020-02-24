//	GCJ 2011 Round 3

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <utility>
#include <numeric>
#include <algorithm>
#include <bitset>
#include <complex>

using namespace std;

typedef unsigned uint;
typedef long long Int;
typedef vector<int> vint;
typedef vector<string> vstr;
typedef pair<int,int> pint;
#define mp make_pair

template<class T> void pv(T a, T b) { for (T i = a; i != b; ++i) cout << *i << " "; cout << endl; }
template<class T> void pvp(T a, T b) { for (T i = a; i != b; ++i) cout << "(" << i->first << ", " << i->second << ") "; cout << endl; }
template<class T> void chmin(T &t, T f) { if (t > f) t = f; }
template<class T> void chmax(T &t, T f) { if (t < f) t = f; }
int in_c() { int c; for (; (c = getchar()) <= ' '; ) { if (!~c) throw ~0; } return c; }
int in() { int x = 0, c; for (; (uint)((c = getchar()) - '0') >= 10; ) { if (c == '-') return -in(); if (!~c) throw ~0; } do { x = (x << 3) + (x << 1) + (c - '0'); } while ((uint)((c = getchar()) - '0') < 10); return x; }
Int In() { Int x = 0, c; for (; (uint)((c = getchar()) - '0') >= 10; ) { if (c == '-') return -In(); if (!~c) throw ~0; } do { x = (x << 3) + (x << 1) + (c - '0'); } while ((uint)((c = getchar()) - '0') < 10); return x; }

int mod(int x, int m) { return (x % m + m) % m; }

int dx[] = { 1, 1, 0, -1, -1, -1, 0, 1, };
int dy[] = { 0, 1, 1, 1, 0, -1, -1, -1, };

const Int MO = 1000003;

int M, N, MN;
char S[110][110];
int A[110][110];
int F[10010][2];
set<int> G[10010];

void ae(int u, int v, int j) {
	F[u][j] = v;
	G[v].insert(u);
}

int que[100010], *qb, *qe;
bool vis[20010];
int uf[20010];
int root(int x) { return (uf[x] < 0) ? x : (uf[x] = root(uf[x])); }
bool conn(int x, int y) {
	x = root(x); y = root(y);
	if (x == y) return 0;
	if (uf[x] > uf[y]) swap(x, y);
	uf[x] += uf[y]; uf[y] = x;
	return 1;
}

Int solve() {
	int u, v, w, j;
	qb = qe = que;
	for (u = 0; u < MN; ++u) {
		if (G[u].size() == 0) return 0;
		if (G[u].size() == 1) {
			*qe++ = u;
		}
	}
	memset(vis, 0, sizeof(vis));
	for (; qb != qe; ) {
		u = *qb++;
		v = *G[u].begin();
		vis[u] = 1;
		vis[MN + v] = 1;
//cout<<"hitori "<<u<<endl;
		for (j = 0; j < 2; ++j) {
			w = F[v][j];
			if (w != u) {
				G[w].erase(v);
				if (!vis[w] && G[w].size() == 0) return 0;
				if (!vis[w] && G[w].size() == 0) *qe++ = w;
			}
		}
	}
	memset(uf, ~0, sizeof(uf));
	for (u = 0; u < MN; ++u) {
		if (!vis[u]) {
			for (set<int>::iterator it = G[u].begin(), en = G[u].end(); it != en; ++it) {
//cout<<"conn "<<u<<" "<<*it<<endl;
				v = *it;
				for (j = 0; j < 2; ++j) {
					conn(MN + v, F[v][j]);
				}
			}
		}
	}
	Int ret = 1;
	for (u = 0; u < MN * 2; ++u) {
		if (!vis[u]) {
			if (uf[u] < 0) {
				int e = -uf[u] / 2;
//cout<<"e = "<<e<<endl;
				Int tmp = (e == 1) ? 1 : 2;
				ret *= tmp;
				ret %= MO;
			}
		}
	}
	return ret;
}

int main() {
	int x, y;
	int g;
	int xx, yy;
	int u;
	
	for (int TC = in(), tc = 0; ++tc <= TC; ) {
		M = in();
		N = in();
		for (x = 0; x < M; ++x) {
			scanf("%s", S[x]);
		}
		for (x = 0; x < M; ++x) for (y = 0; y < N; ++y) {
			switch (S[x][y]) {
				case '|': A[x][y] = 0; break;
				case '\\': A[x][y] = 1; break;
				case '-': A[x][y] = 2; break;
				case '/': A[x][y] = 3; break;
				default: assert (false);
			}
		}
		MN = M * N;
		for (u = 0; u < MN; ++u) {
			G[u].clear();
		}
		for (x = 0; x < M; ++x) for (y = 0; y < N; ++y) {
			g = A[x][y];
			xx = mod(x + dx[g], M);
			yy = mod(y + dy[g], N);
			ae(x * N + y, xx * N + yy, 0);
			g |= 4;
			xx = mod(x + dx[g], M);
			yy = mod(y + dy[g], N);
			ae(x * N + y, xx * N + yy, 1);
		}
		Int res = solve();
		printf("Case #%d: %lld\n", tc, res);
	}
	
	return 0;
}

