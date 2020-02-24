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

int n;
int m;

ll x[5000];
ll y[5000];
ll z[5000];

ll scal (ll x1, ll y1, ll z1, ll x2, ll y2, ll z2) {
	re x1 * x2 + y1 * y2 + z1 * z2;
}

pair<pair<ll, ll>, ll> start;

bool ls (pair<pair<ll, ll>, ll> a, pair<pair<ll, ll>, ll> b) {
    ld cx, cy, cz;
	cx = (ld)a.fi.se * b.se - (ld)a.se * b.fi.se;
	cy = (ld)a.se * b.fi.fi - (ld)a.fi.fi * b.se;
	cz = (ld)a.fi.fi * b.fi.se - (ld)a.fi.se * b.fi.fi;
	if (int (cx > -1e-1) != int (start.fi.fi <= 0) || int (cy > -1e-1) != int (start.fi.se <= 0) || int (cz > -1e-1) != int (start.se <= 0))
		return true;
	else
		return false;
}

int main () {
	int tt;
	cin >> tt;
	for (int it = 1; it <= tt; it++) {
		cin >> n;
		for (int i = 0; i < n; i++) {
			cin >> x[i] >> y[i] >> z[i];
			ll d = gcd (gcd (abs (x[i]), abs (y[i])), abs (z[i]));
			x[i] /= d;
			y[i] /= d;
			z[i] /= d;
		}	
		int ok = 1;
		for (int i = 0; i < n; i++) {
			if (i % 500 == 0)
				cerr << i << endl;
			int anti = 0;
			vector<pair<pair<ll, ll>, ll> > w;
			for (int j = 0; j < n; j++)
				if (i != j) {
					ll cx, cy, cz;
					cx = y[i] * z[j] - z[i] * y[j];
					cy = z[i] * x[j] - x[i] * z[j];
					cz = x[i] * y[j] - y[i] * x[j];
					if (cx == 0 && cy == 0 && cz == 0) {
						if (scal (x[i], y[i], z[i], x[j], y[j], z[j]) < 0) {
							anti = 1;
						}
					} else {
						ll d = gcd (gcd (abs (cx), abs (cy)), abs (cz));
						cx /= d;
						cy /= d;
						cz /= d;
						w.pb (mp (mp (cx, cy), cz));
					}
				}
			if (anti) continue;
			sort (all (w));
			w.resize (unique (all (w)) - w.begin ());
			start = mp (mp (x[i], y[i]), z[i]);
			vector<pair<pair<ll, ll>, ll> > up, down;
			int bad = 0;
			int ok2 = 0;
			down.pb (w[0]);
			for (int k = 1; k < sz (w); k++) {
				if (w[0].fi.fi == -w[k].fi.fi && w[0].fi.se == -w[k].fi.se && w[0].se == -w[k].se) {
					ok2 = 1;
					break;
				}
                ld cx, cy, cz;
				cx = (ld)w[0].fi.se * w[k].se - (ld)w[0].se * w[k].fi.se;
				cy = (ld)w[0].se * w[k].fi.fi - (ld)w[0].fi.fi * w[k].se;
				cz = (ld)w[0].fi.fi * w[k].fi.se - (ld)w[0].fi.se * w[k].fi.fi;
				if (int (cx > -1e-1) != int (x[i] <= 0) || int (cy > -1e-1) != int (y[i] <= 0) || int (cz > -1e-1) != int (z[i] <= 0))
					up.pb (w[k]);
				else
					down.pb (w[k]);	
			}
//			printf ("start: %d %d %d\n", (int)x[i], (int)y[i], (int)z[i]);
			if (!ok2) {
				sort (all (up), ls);
				sort (all (down), ls);
				w.clear ();
				for (int j = 0; j < sz (down); j++) w.pb (down[j]);
				for (int j = 0; j < sz (up); j++) w.pb (up[j]);
//				for (int j = 0; j < sz (w); j++) printf ("%d %d %d\n", (int)w[j].fi.fi, (int)w[j].fi.se, (int)w[j].se);
//				printf ("\n");
				if (sz (w) == 1) {
					bad = 1;
				} else {
					w.pb (w[0]);
					for (int j = 0; j + 1 < sz (w); j++) {
						int k = j + 1;
						if (w[j] == w[k]) continue;
						if (w[j].fi.fi == -w[k].fi.fi && w[j].fi.se == -w[k].fi.se && w[j].se == -w[k].se)
							break;
		                ld cx, cy, cz;
						cx = (ld)w[j].fi.se * w[k].se - (ld)w[j].se * w[k].fi.se;
						cy = (ld)w[j].se * w[k].fi.fi - (ld)w[j].fi.fi * w[k].se;
						cz = (ld)w[j].fi.fi * w[k].fi.se - (ld)w[j].fi.se * w[k].fi.fi;
//						printf ("- %d %d %d\n", (int)cx, (int)cy, (int)cz);
						if (int (cx > -1e-1) != int (x[i] >= 0) || int (cy > -1e-1) != int (y[i] >= 0) || int (cz > -1e-1) != int (z[i] >= 0)) {
							bad = 1;
							break;
						}
					}
				}
			}
			if (bad) {
				ok = 0;
				break;
			}
		}
		cout << "Case #" << it << ": ";
		if (ok) cout << "YES"; else cout << "NO";
		cout << endl;
		fprintf (stderr, "%d / %d = %.2f | %.2f\n", it, tt, (double)clock () / CLOCKS_PER_SEC, ((double)clock () / it * tt) / CLOCKS_PER_SEC);
	}
	return 0;
}