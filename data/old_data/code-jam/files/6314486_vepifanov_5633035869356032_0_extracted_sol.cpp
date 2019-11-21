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

ii gp[60000][13];
ii cur[4][13];
int len[4];
set<vi> all;

int go () {
/*	for (int i = 0; i < n; i++) {
		printf ("%d: ", i);
		for (int j = 0; j < len[i]; j++)
			printf ("%d,%d ", cur[i][j].fi, cur[i][j].se);
		printf ("\n");
	}
	printf ("\n");*/
	int ok = 1;
	for (int i = 0; i < n; i++)
		if (len[i] > 1)
			ok = 0;
	if (ok) re 1;
	vi v;
	for (int i = 0; i < n; i++) {
		v.pb (len[i]);
		for (int j = 0; j < len[i]; j++) {	
			v.pb (cur[i][j].fi);
			v.pb (cur[i][j].se);
		}
	}
	if (all.count (v)) re 0;
	all.insert (v);
	for (int i = 0; i < n; i++)
		if (len[i] == 0) {
			for (int j = 0; j < n; j++)
				if (len[j] > 1) {
					ii tmp = cur[j][len[j] - 1];
					len[j]--;
					cur[i][len[i]] = tmp;
					len[i]++;
					if (go ()) re 1;
					len[i]--;
					cur[j][len[j]] = tmp;
					len[j]++;
				}
			break;
		}
	for (int i = 0; i < n; i++) {
		if (len[i] == 0) continue;
		int last = -1, first = -1, cnt = 0;
		for (int j = 0; j < n; j++)
			if (len[j] > 0 && cur[j][len[j] - 1].se == cur[i][len[i] - 1].se) {
				cnt++;
				if (first == -1) first = j;
				if (last == -1 || cur[last][len[last] - 1].fi > cur[j][len[j] - 1].fi)
					last = j;
			}
		if (cnt > 1 && first == i) {
			ii tmp = cur[last][len[last] - 1];
			len[last]--;
			if (go ()) re 1;
			cur[last][len[last]] = tmp;
			len[last]++;
		}
	}
	re 0;
}

int main () {
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> m;
		for (int j = m - 1; j >= 0; j--)
			cin >> gp[i][j].fi >> gp[i][j].se;
	}
	int tt;
	cin >> tt;
	for (int it = 1; it <= tt; it++) {
		cin >> n >> m;
		for (int i = 0; i < n; i++) {
			int k;
			cin >> k;
			for (int j = 0; j < m; j++)
				cur[i][j] = gp[k][j];
			len[i] = m;
		}
		all.clear ();
		cout << "Case #" << it << ": " << (go () ? "POSSIBLE" : "IMPOSSIBLE");
		cout << endl;
		fprintf (stderr, "%d / %d = %.2f | %.2f\n", it, tt, (double)clock () / CLOCKS_PER_SEC, ((double)clock () / it * tt) / CLOCKS_PER_SEC);
	}
	return 0;
}