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

int n;
int m;

int x[5000];
int y[5000];

int main () {
	int tt;
	cin >> tt;
	for (int it = 1; it <= tt; it++) {
		cin >> n >> m;		
		for (int i = 0; i < n; i++) cin >> x[i];
		int ans = 0, ok = 1;
		while (m >= 1) {	
			for (int i = 0; i < n; i++)
				if (x[i] != x[i % (2 * m)])	{
					ok = 0;
					break;
				}
			n = 2 * m;
			for (int i = 0; i < n; i++) y[i] = x[i];
			for (int i = 0; i < n; i++) {
				int a = x[(i + m) % n] - x[i];
				int b = x[(i + 1 + m) % n] - x[(i + 1) % n];
				int c = b - a;
				if (c & 1) {
					ok = 0;
					break;
				}
				if (c > 0) {
					ans += c / 2;
					for (int j = 1; j <= m; j++)
						y[(i + j) % n] += c / 2; 
				}
			}
			if (!ok) break;
			for (int i = 0; i < n; i++) x[i] = y[i];
			m /= 2;
		}
		cout << "Case #" << it << ": ";
		if (!ok) cout << "CHEATERS!"; else cout << ans;
		cout << endl;
		fprintf (stderr, "%d / %d = %.2f | %.2f\n", it, tt, (double)clock () / CLOCKS_PER_SEC, ((double)clock () / it * tt) / CLOCKS_PER_SEC);
	}
	return 0;
}