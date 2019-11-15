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
#define sqrt(x) sqrt(abs(x))
#define unq(x) (x.resize(unique(all(x)) - x.begin()))
#define spc(i,n) " \n"[i == n - 1]

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<string> vs;
typedef double D;
typedef long double LD;
typedef long long ll;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

template<class T> T abs(T x) { return x > 0 ? x : -x;}

int m;
int n;

int w[1 << 20];

double table[1 << 20];

double getans(int x) {
	if (x == (1 << n) - 1)
		re 0;
	if (w[x])
		re table[x];
	w[x] = 1;
	double &ans = table[x];
	ans = 0;
	rep(i, n)
	if ((x & (1 << i)) == 0) {
		int nx = x + (1 << i);
		int s = n;
		int p = (i + 1) % n;
		int c = 1;
		while (x & (1 << p)) {
			s += n - c;
			c++;
			p = (p + 1) % n;
		}
		ans += getans(nx) * c / (double) n + s / (double) n;
	}
	re ans;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tc;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		string s;
		cin >> s;
		reverse(all(s));
		n = sz(s);
		int c = 0;
		rep(i, sz(s)) {
			if (s[i] == 'X')
				c += (1 << i);
		}
		fill(w, 0);
		printf("%.10lf", getans(c));
		cout << endl;
	}



	re 0;
}

