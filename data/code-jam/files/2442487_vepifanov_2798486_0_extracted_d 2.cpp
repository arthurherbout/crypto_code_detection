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

const int MAX = 20000000;

ll n, m;
long long a, b, x, y, vx, vy, v, w;
int w1[1000000];
long long y1[1000000];
long long t1[1000000];
int w2[1000000];
long long y2[1000000];
long long t2[1000000];
//pair<ll, ll> q[MAX];

int main () {
	int tt;
	cin >> tt;
	for (int it = 1; it <= tt; it++) {

		cin >> a >> b >> n >> m >> v >> w >> y >> x >> vy >> vx;
		cout << "Case #" << it << ": ";

		if (vx == 0)
			cout << "DRAW";
		else {	
			int rev = 0;
			if (vx < 0) {
				rev = 1;
				vx = -vx;
				x = b - x;
				swap (n, m);
				swap (v, w);
			}
			if (vy < 0) {
				vy = -vy;
				y = a - y;
			}
			y *= vx;
			for (int i = 0; i < n; i++) {
				w1[i] = 0;
				y1[i] = 0;
				t1[i] = 0;
			}
			for (int i = 0; i < m; i++) {
				w2[i] = 0;
				y2[i] = 0;
				t2[i] = 0;
			}
			int c1 = 0, c2 = 0, bad = 0;
			long long ct = 0;
			int per = -1;
			for (int i = 0; i < MAX; i++)
				if (i & 1) {
					long long tmp = x;
					ct += tmp;
					x = 0;
					y = (y + tmp * vy);
					y %= 2 * a * vx;
					long long ry = min (y, 2 * a * vx - y);
					int k = c1 % n;
//					printf ("left: %d = %d = %.10f %.10f %.10f\n", c1, k, ry, y1[k], (ct - t1[k]) * v);
					if (w1[k]) {
						if (abs (y1[k] - ry) > (ct - t1[k]) * v) {
							bad = -1;
							break;
						}
					}	
					w1[k] = 1;
					y1[k] = ry;
					t1[k] = ct;
					c1++;
				} else {
					long long tmp = b - x;
					ct += tmp;
					x = b;
					y = (y + tmp * vy);
					y %= 2 * a * vx;
					long long ry = min (y, 2 * a * vx - y);
					int k = c2 % m;
//					printf ("right: %d = %d = %.10f %.10f %.10f\n", c2, k, ry, abs (ry - y2[k]), (ct - t2[k]) * w);
					if (w2[k]) {
						if (abs (y2[k] - ry) > (ct - t2[k]) * w) {
							bad = 1;
							break;
						}
					}
					w2[k] = 1;
					y2[k] = ry;
					t2[k] = ct;
					c2++;
				}
			if (rev) {
				bad = -bad;
				swap (c1, c2);
			}
//			cerr << per << endl;
			if (bad == 0) cout << "DRAW"; else
			if (bad == 1) cout << "LEFT " << c2; else cout << "RIGHT " << c1;
		}	
		cout << endl;

		cerr << it << " / " << tt << " " << clock () << " " << endl;
	}
	return 0;
}