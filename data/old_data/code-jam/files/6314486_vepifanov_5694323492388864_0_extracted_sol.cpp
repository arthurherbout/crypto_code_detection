#include <bits/stdc++.h>

using namespace std;

#define mp make_pair
#define pb push_back
#define rep(i,n) for(int i = 0; i < (n); i++)
#define re return
#define fi first
#define se second
#define sz(x) ((int) (x).size())
#define all(x) (x).begin(), (x).end()
#define sqrt(x) sqrt(abs(x))
#define y0 y3487465
#define y1 y8687969
#define fill(x,y) memset(x,y,sizeof(x))
#define prev PREV
                         
typedef vector<int> vi;
typedef long long ll;
typedef long double ld;
typedef double D;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<string> vs;
typedef vector<vi> vvi;

template<class T> T abs(T x) { re x > 0 ? x : -x; }
template<class T> T sqr(T x) { re x * x; }
template<class T> T gcd(T a, T b) { re a ? gcd (b % a, a) : b; }

const int MAX = 6500;

int n;
int m;
int x[102];
int y[102];
int z[102];
int was[102][MAX];
int d[102][MAX];
int p[102][MAX];
ii q[102 * MAX];

int gp (int i, int x) {
	if (x >= MAX) re x;
	if (was[i][x]) re p[i][x] = gp (i, p[i][x]);
	re x;
}

ii calc (int x, int y, int z, int d, int xx, int yy, int zz) {
	int a = 1e9;
	int b = 0;
	for (int t = 0; t < 6; t++) {
		int c = abs (x + (int (t == 0) - int (t == 1)) * d - xx) +
				abs (y + (int (t == 2) - int (t == 3)) * d - yy) + 
				abs (z + (int (t == 4) - int (t == 5)) * d - zz);
		a = min (a, c);
		b = max (b, c);
	}
	a = min (a, abs (abs (x - xx) + abs (y - yy) + abs (z - zz) - d));
	re mp (a, b);
}

int main () {
	int tt;
	cin >> tt;
	for (int it = 1; it <= tt; it++) {
		cin >> n; n += 2;
		for (int i = 0; i < n; i++) cin >> x[i] >> y[i] >> z[i];
		memset (was, 0, sizeof (was));
		int l = 0, r = 0;
//		if (abs (x[0] + y[0] + z[0]) % 2 == abs (x[1] + y[1] + z[1]) % 2) {
			for (int i = 0; i < n; i++)
				for (int j = 0; j < MAX; j++)
					p[i][j] = j;
			for (int i = 2; i < n; i++) {
				ii seg = calc (x[0], y[0], z[0], 0, x[i], y[i], z[i]);
				q[r++] = mp (i, seg.fi);
				was[i][seg.fi] = 1;
				d[i][seg.fi] = 0;
				p[i][seg.fi] = seg.fi + 1;
			}
			while (l < r) {
				ii p = q[l++];
	//			if (l % 10000 == 0) cerr << l << " " << r << endl;
//				printf ("%d %d = %d\n", p.fi, p.se, d[p.fi][p.se]);
				if (p.fi != 1)
					for (int i = 1; i < n; i++) {
						ii seg = calc (x[p.fi], y[p.fi], z[p.fi], p.se, x[i], y[i], z[i]);
						seg.se = min (seg.se, MAX - 1);
	//					printf ("-> %d | %d %d\n", i, seg.fi, seg.se);
						int j = gp (i, seg.fi);
						while (j <= seg.se) {
							was[i][j] = 1;
							::p[i][j] = j + 1;
							d[i][j] = d[p.fi][p.se] + 1;
							q[r++] = mp (i, j);
							j = gp (i, j);
						}
				}
			}
	/*		for (int i = 0; i < n; i++) {
				printf ("%d: ", i);
				for (int j = 0; j < MAX; j++)
					if (!was[i][j])
						printf ("%d ", j);
				printf ("\n");
			}
			cerr << r << endl;*/
//		}	
		cout << "Case #" << it << ": ";
		if (was[1][0]) cout << d[1][0]; else cout << "IMPOSSIBLE";// << (r ? "" : " FAST");
		cout << endl;
		fprintf (stderr, "%d / %d = %.2f | %.2f\n", it, tt, (double)clock () / CLOCKS_PER_SEC, ((double)clock () / it * tt) / CLOCKS_PER_SEC);
	}
	return 0;
}