#ifndef LOCAL_BOBER
#pragma comment(linker, "/STACK:134217728")
#endif

#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <bitset>
#include <queue>
#include <stack>
#include <sstream>
#include <cstring>
#include <numeric>
#include <ctime>

#define re return
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int) (x).size())
#define rep(i, n) for (int i = 0; i < (n); i++)
#define rrep(i, n) for (int i = (n) - 1; i >= 0; i--)
#define y0 y32479
#define y1 y95874
#define fill(x, y) memset(x, y, sizeof(x))
#define sqr(x) ((x) * (x))
#define prev prev239
#define next next239
#define hash hash239
#define rank rank239
#define sqrt(x) sqrt(abs(x))

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<string> vs;
typedef long long ll;
typedef double D;
typedef long double LD;

template<class T> T abs(T x) {return x > 0 ? x : -x;}

int n;
int m;

int dx[100][100];
int dy[100][100];

int c[100][100];

int zlo;

ii get(int x, int y, int p) {
	int nx = x + dx[x][y] * p;
	int ny = y + dy[x][y] * p;
	if (nx < 0) nx += n;
	if (nx >= n) nx -= n;
	if (ny < 0) ny += m;
	if (ny >= m) ny -= m;
	re mp(nx, ny);
}

int main() {
#ifdef LOCAL_BOBER
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif

	int tc;
	cin >> tc;
	rep(tt, tc) {
		printf("Case #%d: ", tt + 1);

		cin >> n >> m;
		rep(i, n) {
			string s;
			cin >> s;
			rep(j, m) {
				dx[i][j] = 0;
				dy[i][j] = 0;
				if (s[j] == '-')
					dy[i][j] = 1;
				if (s[j] == '|')
					dx[i][j] = 1;
				if (s[j] == '\\') {
					dx[i][j] = 1;
					dy[i][j] = 1;
				}
				if (s[j] == '/') {
					dx[i][j] = 1;
					dy[i][j] = -1;
				}
			}
		}

		int ans = 0;
		zlo = 0;

		rep(t, (1 << (n * m))) {
			zlo++;
			rep(i, n) rep(j, m) {
				ii tmp;
				if (t & (1 << ((i * m) + j)))
					tmp = get(i, j, -1);
				else
					tmp = get(i, j, 1);
				/*if (t == 0) {
					cout << i << ' ' << j << ' ' << dx[i][j] << ' ' << dy[i][j] << ' ' << tmp.fi << ' ' << tmp.se << endl;
				}*/
				c[tmp.fi][tmp.se] = zlo;
			}
			int f = 1;
			rep(i, n) rep(j, m)
				if (c[i][j] != zlo)
					f = 0;
			ans += f;
		}
		cout << ans << endl;
	}

	re 0;
}

