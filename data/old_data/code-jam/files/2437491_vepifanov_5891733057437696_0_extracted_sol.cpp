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

const double eps = 1e-10;

int n;
int m;
int x[10000];
int y[10000];
double xx[10000], yy[10000];
double ax, ay, bx, by;
double w[40000];

int can (double ang) {
	double x1 = cos (ang), y1 = -sin (ang);
	double x2 = sin (ang), y2 = cos (ang);
//	printf ("%.10f %.10f - %.10f %.10f\n", x1, y1, x2, y2);
	for (int i = 0; i < n; i++) {
		xx[i] = x[i] * x1 + y[i] * y1;
		yy[i] = x[i] * x2 + y[i] * y2; 
	}
	sort (xx, xx + n);
	sort (yy, yy + n);
	if (xx[n / 2] - xx[n / 2 - 1] < eps) re 0;
	if (yy[n / 2] - yy[n / 2 - 1] < eps) re 0;
	double cx = (xx[n / 2] + xx[n / 2 - 1]) / 2;
	double cy = (yy[n / 2] + yy[n / 2 - 1]) / 2;
//	printf ("%.10f %.10f\n", cx, cy);
	int c[4];
	memset (c, 0, sizeof (c));
	for (int i = 0; i < n; i++) {
		double xx = x[i] * x1 + y[i] * y1;
		double yy = x[i] * x2 + y[i] * y2; 
//		printf ("%.10f %.10f\n", xx - cx, yy - cy);
		c[int (xx > cx) + 2 * int (yy > cy)]++;
	}
	ax = cx * x1 + cy * y1;
	ay = cx * x2 + cy * y2;
	bx = ax + x1;
	by = ay + y1;
	cx = ax + x2;
	cy = ay + y2;
	int w[4];
	memset (w, 0, sizeof (w));
/*	for (int i = 0; i < n; i++) {
		double xx = (x[i] - ax) * (by - ay) - (y[i] - ay) * (bx - ax);
		double yy = (x[i] - ax) * (cy - ay) + (y[i] - ay) * (cx - ax);
//		printf ("%.10f %.10f\n", xx, yy);
		w[int (xx > 0) * 2 + int (yy > 0)]++;
	}*/
/*	for (int i = 0; i < 4; i++) {
		printf ("%d %d\n", w[i], c[i]);
		if (w[i] != c[i])
			cerr <<"bad" << endl;
	}*/
	re int (c[0] == c[1] && c[1] == c[2] && c[2] == c[3]);
}

int main () {
	int tt;
	cin >> tt;
	for (int it = 1; it <= tt; it++) {
		scanf ("%d", &n); n *= 4;
		for (int i = 0; i < n; i++) scanf ("%d%d", &x[i], &y[i]);
		m = 0;
		for (int i = 0; i < n; i++)
			for (int j = i + 1; j < n; j++) {
				w[m++] = atan2 (y[j] - y[i], x[j] - x[i]);
				w[m++] = atan2 (x[i] - x[j], y[j] - y[i]);
				w[m++] = atan2 (y[i] - y[j], x[i] - x[j]);
				w[m++] = atan2 (x[j] - x[i], y[i] - y[j]);
				w[m++] = -atan2 (y[j] - y[i], x[j] - x[i]);
				w[m++] = -atan2 (x[i] - x[j], y[j] - y[i]);
				w[m++] = -atan2 (y[i] - y[j], x[i] - x[j]);
				w[m++] = -atan2 (x[j] - x[i], y[i] - y[j]);
			}
		sort (w, w + m);
		int ok = 0;
		for (int i = 0; i < m; i++) {
//			printf ("angs %.10f %.10f\n", w[i] / asin (1.0) * 90, w[(i + 1) % m] / asin (1.0) * 90);
			if (can ((w[i] + w[(i + 1) % m]) / 2)) {
				ok = 1;
				break;
			}
		}	

		cout << "Case #" << it << ": ";

		if (!ok) printf ("IMPOSSIBLE"); else printf ("%.8f %.8f %.8f %.8f", (double)ax, (double)ay, (double)bx, (double)by);

		cout << endl;

		cerr << it << " / " << tt << " " << clock () << endl;
	}
	return 0;
}