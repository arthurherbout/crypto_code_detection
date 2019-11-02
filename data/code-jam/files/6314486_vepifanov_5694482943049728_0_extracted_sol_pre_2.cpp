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

// there was a huge precalc here

int main () {
	int tt;
	cin >> tt;
	for (int it = 1; it <= tt; it++) {
		cin >> n;
		cout << "Case #" << it << ": " << res[n].fi << endl;
		for (int i = 0; i < res[n].fi; i++) {
			for (int j = 0; j < res[n].fi; j++)
				printf ("%c", res[n].se[i * res[n].fi + j]);
			printf ("\n");
		}
		fprintf (stderr, "%d / %d = %.2f | %.2f\n", it, tt, (double)clock () / CLOCKS_PER_SEC, ((double)clock () / it * tt) / CLOCKS_PER_SEC);
	}
	return 0;
}