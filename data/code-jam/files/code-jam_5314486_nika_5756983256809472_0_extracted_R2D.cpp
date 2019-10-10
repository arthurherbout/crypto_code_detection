#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <sstream>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
#define SZ(x) (int)(x.size())
#define F0(i,n) for(int i=0;i<n;i++)
#define F1(i,n) for(int i=1;i<=n;i++)
const int MOD = 1000002013;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

int i, j, k, m, n, l, sn, tn;
int sx[101], sy[101], tx[101], ty[101], id[101][101], shoots[101][101][101];
string s[101];

int d[1 << 10][1 << 10], d1[1 << 10][1<<10], d2[1 << 10][1<<10], destroy[101][101];
int v[101][101];

const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

int main() {
    //freopen("x.in", "r", stdin);

	freopen("D-small-attempt0.in", "r", stdin);
	freopen("D-small-attempt0.out", "w", stdout);

	//freopen("D-large.in", "r", stdin);
	//freopen("D-large.out", "w", stdout);

	int tt, ttt; cin >> ttt;

	F1(tt,ttt) {
		cerr << tt << endl;

		cin >> n >> m >> l;
		F0(i, m) cin >> s[i];

		sn = 0;
		F0(i, m) F0(j, n) if (s[i][j] == 'S') {
			sx[sn] = i; sy[sn] = j; id[i][j] = sn; sn++;
		}
		tn = 0;
		F0(i, m) F0(j, n) if (s[i][j] == 'T') {
			tx[tn] = i; ty[tn] = j; id[i][j] = tn; tn++;
		}

		F0(t, tn) {
			F0(i, m) F0(j, n) shoots[t][i][j] = 0;
			F0(k, 4) {
				int x = tx[t], y = ty[t];
				while (1) {
					x += dx[k];
					y += dy[k];
					if (x < 0 || x >= m || y < 0 || y >= n) break;
					if (s[x][y] == '#') break;
					shoots[t][x][y] = 1;
				}
			}
		}

		memset(d, -1, sizeof(d));

		d[(1<<tn)-1][(1<<sn)-1] = 0;
		int ans = 0, at = (1<<tn)-1, as = (1<<sn)-1;
		
		for (int tmask = (1<<tn)-1; tmask >= 0; tmask--) {

			F0(si, sn) {
				F0(t, tn) destroy[si][t] = 0;
				queue<pii> Q;
				memset(v, -1, sizeof(v));
				v[sx[si]][sy[si]] = 0;
				Q.push(pii(sx[si], sy[si]));
				while (!Q.empty()) {
					int x = Q.front().first;
					int y = Q.front().second;
					Q.pop();
					int freeze = (v[x][y] == l);
					F0(t, tn) if ((1 << t)&tmask) {
						if (shoots[t][x][y]) {
							destroy[si][t] = 1;
							freeze = 1;
						}
					}
					if (!freeze) {
						F0(k, 4) {
							int xx = x + dx[k];
							int yy = y + dy[k];
							if (xx < 0 || xx >= m || yy < 0 || yy >= n) continue;
							if (s[xx][yy] == '#' || v[xx][yy] != -1) continue;
							v[xx][yy] = v[x][y] + 1;
							Q.push(pii(xx, yy));
						}
					}
				}
			}
			F0(smask, (1 << sn)) if (d[tmask][smask] >= 0) {
				F0(s, sn) if ((1 << s)&smask) {
					F0(t, tn) if (((1 << t) & tmask) && destroy[s][t]) {
						// kill t with s
						d[tmask ^ (1 << t)][smask ^ (1 << s)] = d[tmask][smask] + 1;
						d1[tmask ^ (1 << t)][smask ^ (1 << s)] = t;
						d2[tmask ^ (1 << t)][smask ^ (1 << s)] = s;
						if (d[tmask ^ (1 << t)][smask ^ (1 << s)] > ans) {
							ans = d[tmask ^ (1 << t)][smask ^ (1 << s)];
							at = tmask ^ (1 << t);
							as = smask ^ (1 << s);
						}
					}
				}
			}
		}
		vector<int> v1, v2;
		while (d[at][as] > 0) {
			int p1 = d1[at][as];
			int p2 = d2[at][as];
			v1.push_back(p2 + 1);
			v2.push_back(p1 + 1);
			at ^= (1 << p1);
			as ^= (1 << p2);
		}
		reverse(v1.begin(), v1.end());
		reverse(v2.begin(), v2.end());

  		printf("Case #%d: %d\n", tt, ans);
		F0(i, ans) {
			cout << v1[i] << " " << v2[i] << endl;
		}
	}
	return 0;
}
