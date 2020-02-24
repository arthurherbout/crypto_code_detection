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

int n;
int m;
vvi all;
int p[8];
vi ans;
int use[8];
set<vi> good;

int check (vi v) {
	int was[8][8];
	memset (was, 0, sizeof (was));
	for (int j = 0; j < sz (all); j++) {
		int ok = 1;
		for (int k = 0; k < n && ok; k++)
			if (all[j][k] != v[k] && v[k] != -1)
				ok = 0;
		if (ok) {
//			printf ("%d %d %d\n", all[j][0], all[j][1], all[j][2]);
			for (int k = 0; k < n && ok; k++)
				if (v[k] == -1)
					was[k][all[j][k]] = 1;
		}
	}
	int ok = 1;
	for (int j = 0; j < n; j++) {
		if (v[j] != -1) continue;
		int s = 0;
		for (int k = 0; k < n; k++)
			s += was[j][k];
		if (s <= 1) { ok = 0; break; }
	}
	re ok;
}

int go (int i, vi v) {
/*	for (int j = 0; j < n; j++) printf ("%d ", v[j]);
	printf ("\n");*/
	if (i == n - 1) {
		v[p[i]] = 0;
		if (v > ans && good.count (v)) ans = v;
		re 0;
	}
	if (!check (v)) re 0;
	for (int j = 0; j < n; j++)
		if (!use[j]) {
			use[j] = 1;
			v[p[i]] = j;
			go (i + 1, v);
			v[p[i]] = -1;
			use[j] = 0;
		}
	re 0;
}

int main () {
	int tt;
	scanf ("%d", &tt);
	for (int it = 1; it <= tt; it++) {
		cerr << it << endl;
		scanf ("%d", &n);
		for (int i = 0; i < n; i++) {
			scanf ("%d", &p[i]);
			p[i]--;
		}
		vi v;
		for (int i = 0; i < n; i++) v.pb (i);
		all.clear ();
		do {
			int ok = 1;
			for (int a = 0; a < n; a++)
				for (int b = a + 1; b < n; b++)
					for (int c = b + 1; c < n; c++)
						if (v[a] > v[b] && v[b] > v[c])
							ok = 0;
			if (ok) {
				all.pb (v);
				good.insert (v);
			}
		} while (next_permutation (all (v)));
//		printf ("%d\n", sz (all));
		for (int i = 0; i < n; i++) v[i] = -1;
		ans = v;
		go (0, v);
		printf ("Case #%d:", it);
		for (int i = 0; i < n; i++)
			printf (" %d", ans[i] + 1);
		printf ("\n");
	}
	return 0;
}
