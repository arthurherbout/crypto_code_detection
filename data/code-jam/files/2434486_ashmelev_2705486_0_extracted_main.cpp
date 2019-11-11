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


char ss[10000];

vs v;

void add(string s) {
	v.pb(s.substr(0, sz(s) - 1));
}

int table[60][6];
string s;

int getans(int p, int t) {
	int &ans = table[p][t];
	if (p == m)
		re 0;

	//cout << p << ' ' << t << endl;
	if (ans != -1)
		re ans;
	ans = 1000;
	rep(i, n) {
		if (sz(v[i]) + p <= m) {
			int g = 1;
			int sum = 0;
			int last = -t;
			//cout << "last = " << last << endl;
			for (int j = 0; j < sz(v[i]); j++) {
				if (v[i][j] != s[p + j]) {
					//cout << "diff: " << j << endl;
					if (j - last < 5) {
						g = 0;
						break;
					}
					//cout << "j = " << j << endl;
					last = j;
					sum++;
				}
			}
			if (g) {
				//cout << "last = " << last << ' ' << sz(v[i]) << ' ' << l1 << endl;
				ans = min(ans, sum + getans(p + sz(v[i]), min(5, sz(v[i]) - last)));
			}
		}
	}
	re ans;
}

int main() {
#ifdef LOCAL_BOBER
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif

	FILE *f = fopen("dict.txt", "r");
	while (fgets(ss, 1000, f)) {
		add(ss);
	}

	n = sz(v);

	/*
	cout << sz(v) << endl;
	cout << v[0] << endl;
	cout << v.back() << endl;
	re 0;*/


	int tc;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";
		string s;
		cin >> s;
		fill(table, -1);
		::s = s;
		m = sz(s);
		cout << getans(0, 5) << endl;
	}

	re 0;
}
