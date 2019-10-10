#include <cstdio>
#include <numeric>
#include <iostream>
#include <vector>
#include <set>
#include <cstring>
#include <string>
#include <map>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <bitset>
#include <queue>
#include <sstream>
#include <deque>

using namespace std;

#define mp make_pair
#define pb push_back
#define rep(i,n) for(int i = 0; i < (n); i++)
#define re return
#define fi first
#define se second
#define sz(x) ((int) (x).size())
#define all(x) (x).begin(), (x).end()
#define sqr(x) ((x) * (x))
#define sqrt(x) sqrt(abs(x))
#define y0 y3487465
#define y1 y8687969
#define fill(x,y) memset(x,y,sizeof(x))
                         
typedef vector<int> vi;
typedef long long ll;
typedef long double ld;
typedef double D;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<string> vs;
typedef vector<vi> vvi;

template<class T> T abs(T x) { re x > 0 ? x : -x; }

const int N = 5;

int n;
int m;
int w[101000][4];
int g[101000][4];
vii v[101001];
vector<pair<int, ii> > v2[101001];
int d;
int K;
set<int> all[101001];
set<ii> all2[101001];

int go (int i, int x) {
	if (i == n) re i;
	int j = -1;
	for (int k = 0; k < d; k++)
		if (g[i][k] == x) {
			j = k;
			break;
		}	
	if (j == -1) re i;
	if (w[i][j] != 0) re w[i][j];
	re w[i][j] = go (i + 1, x);
}

int go (int i, int x, int y) {
	while (i < n) {
		int j = i;
		j = go (j, x);
		j = go (j, y);
		if (j == i) break;
		i = j;
	}
	re i;
}

int go (int i, int x, int y, int z) {
	while (i < n) {
		int j = i;
		j = go (j, x);
		j = go (j, y);
		j = go (j, z);
		if (j == i) break;
		i = j;
	}
	re i;
}

int check (int i, int x, int y, int z) {
	for (int j = 0; j < d; j++)
		if (g[i][j] == x || g[i][j] == y || g[i][j] == z)
			re 1;
	re 0;
}

int main () {
	int tt;
	cin >> tt;
	for (int it = 1; it <= tt; it++) {
		scanf ("%d%d%d", &n, &d, &K); K = 3;
		for (int i = 0; i < n; i++) {
			v[i].clear ();
			for (int j = 0; j < d; j++) {
				scanf ("%d", &g[i][j]);
				w[i][j] = 0;
			}	
			sort (g[i], g[i] + d);
		}
		int ai = 0, aj = 0;
		if (K == 3) {
			for (int i = 0; i < n; i++) {
				if (n - i - 1 <= aj - ai) break;
				for (int a = 0; a < d; a++)
					if (a == 0 || g[i][a] != g[i][a - 1]) {
						int ok = 1;
						if (i > 0)
							for (int j = 0; j < d; j++)
								if (g[i - 1][j] == g[i][a])
									ok = 0;
						if (!ok) continue;
						int j = go (i, g[i][a]);
						if (j - i - 1 > aj - ai) {
							ai = i;
							aj = j - 1;
						}
//						printf ("%d %d %d\n", i, A, j);
						if (j < n)
						for (int b = 0; b < d; b++)
							if (b == 0 || g[j][b] != g[j][b - 1]) {
								int A = g[i][a];
								int B = g[j][b];
								if (all[A].find (B) != all[A].end ()) continue;
								int k = go (j, A, B);
								if (k - i - 1 > aj - ai) {
									ai = i;
									aj = k - 1;
								}
//								printf ("%d %d %d -> %d\n", i, A, B, k - 1);
								if (k < n)
								for (int c = 0; c < d; c++)
									if (c == 0 || g[k][c] != g[k][c - 1]) {
										int C = g[k][c];
										if (all2[A].find (mp (B, C)) != all2[A].end ()) continue;
										int f = k;
										int ok = 1;
										for (int T = 0; T < N; T++)
											if (i + (aj - ai) + 1 - T > f && !check (i + (aj - ai) + 1 - T, A, B, C)) {
												ok = 0;
												break;
											}
										if (ok) f = go (f, A, B, C);
//										int f = go (k, A, B, C);
//										printf ("%d %d %d %d -> %d\n", i,A ,B ,C ,f - 1);
										if (f - i - 1 > aj - ai) {
											ai = i;
											aj = f - 1;
										}
										v2[f].pb (mp (A, mp (B, C)));
										all2[A].insert (mp (B, C));
									}
								v[k].pb (mp (A, B));
								all[A].insert (B);
							}
					}
				for (int j = 0; j < sz (v[i]); j++)
					all[v[i][j].fi].erase (v[i][j].se);
				for (int j = 0; j < sz (v2[i]); j++)
					all2[v2[i][j].fi].erase (v2[i][j].se);
			}
		} else {
			for (int i = 0; i < n; i++) {
				for (int a = 0; a < d; a++)
					if (a == 0 || g[i][a] != g[i][a - 1]) {
						int j = go (i, g[i][a]);
						if (j - i - 1 > aj - ai) {
							ai = i;
							aj = j - 1;
						}
						if (j < n)
						for (int b = 0; b < d; b++)
							if (b == 0 || g[j][b] != g[j][b - 1]) {
								int A = g[i][a];
								int B = g[j][b];
								if (all[A].find (B) != all[A].end ()) continue;
								int k = go (j, A, B) - 1;
								if (k - i > aj - ai) {
									ai = i;
									aj = k;
								}
								v[k].pb (mp (A, B));
								all[A].insert (B);
							}
					}
				for (int j = 0; j < sz (v[i]); j++)
					all[v[i][j].fi].erase (v[i][j].se);
			}
		}
		cout << "Case #" << it << ": " << ai << " " << aj;

		cout << endl;

		cerr << it << " / " << tt << " " << clock () << endl;
	}
	return 0;
}