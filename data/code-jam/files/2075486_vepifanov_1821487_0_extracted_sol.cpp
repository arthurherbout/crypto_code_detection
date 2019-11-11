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

const int INF = 1e9;
const int MAX = 1000;

int n;
int m;
int x[1000], y[1000], z[1000];

int dist (int a, int b) { re abs (a) + abs (b); }

int main () {
	double start = clock ();
	double last = clock ();
	int tt;
	cin >> tt;

	for (int it = 1; it <= tt; it++) {
			
		scanf ("%d", &n);

		for (int i = 0; i < n; i++) {
			string s;
			cin >> x[i] >> y[i] >> s;
			z[i] = int (s == "#");
		}	

		int xx = INF, yy = INF;
			
		cout << "Case #" << it << ": ";

		for (int i = -MAX; i <= MAX; i++)
			for (int j = -MAX; j <= MAX; j++) {
				int ok = 1;
				for (int k = 0; k < n; k++)
					if ((max (abs (x[k] - i), abs (y[k] - j)) & 1) != z[k]) {
						ok = 0;
						break;
					}
				if (ok && (dist (xx, yy) > dist (i, j) || (dist (xx, yy) == dist (i, j) && (mp (i, j) > mp (xx, yy))))) {
					xx = i;
					yy = j;
				}
			}

		if (xx == INF) printf ("Too damaged"); else printf ("%d %d", xx, yy);

		cout << endl;
		double current = clock ();
		fprintf (stderr, "%d / %d = %.10f : %.10f\n", it, tt, (current - last) / CLOCKS_PER_SEC, (current - start) / CLOCKS_PER_SEC);
		last = current;
	}
	return 0;
}