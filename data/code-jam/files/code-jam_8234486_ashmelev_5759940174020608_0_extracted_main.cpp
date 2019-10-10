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
#define prev prev239
#define next next239
#define hash hash239
#define rank rank239
#define sqrt(x) sqrt(abs(x))
#define unq(x) (x.resize(unique(all(x)) - x.begin()))

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<string> vs;
typedef long long ll;
typedef double D;
typedef long double LD;
typedef pair<ll, ll> pll;

template<class T> T abs(T x) {
	return x > 0 ? x : -x;
}

int n;

map<string, int> m;

char s[1000500];

int cp;

int add(string s) {
	if (m.count(s) == 0)
		m[s] = cp++;
	re m[s];
}

vi parse(string s) {
	stringstream sin(s);
	string tmp;
	vi ans;
	while (sin >> tmp) {
		ans.pb(add(tmp));
	}
	re ans;
}

int z;

int val1[100500];
int val2[100500];

vvi zlo;

void make(int id, int val, int f) {
	rep(i, sz(zlo[id])) {
		int x = zlo[id][i];
		if (f == 1) {
			if (val1[x] == 3)
				continue;
			val1[x] |= (1 << val);
			if (val1[x] == 3)
				z++;
		}
		else
		if (f == 0) {
			if (val2[x] == 3 || val1[x] == 3)
				continue;
			int tmp = val2[x] | (1 << val);
			tmp |= val1[x];
			val2[x] = tmp;
			if (tmp == 3)
				z++;
		}
		else
		if (f == -1) {
			val2[x] = 0;
		}
	}
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tc;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		cin >> n;
		cp = 0;
		m.clear();
		gets(s);
		zlo.clear();
		rep(i, n) {
			gets(s);
			vi v = parse(s);
			zlo.pb(v);
		}

		fill(val1, 0);
		fill(val2, 0);

		z = 0;
		int ans = 1000000;
		make(0, 0, 1);
		make(1, 1, 1);
		int zz = z;
		/*cout << z << endl;
		cout << cp << endl;
		rep(i, cp)
			cout << val1[i] << ' ';
		cout << endl;*/
		rep(i, 1 << n) {
			z = zz;
			if (i & 1)
				continue;
			if ((i & 2) == 0)
				continue;
			for (int j = 2; j < n; j++) {
				if (i & (1 << j))
					make(j, 1, 0);
				else
					make(j, 0, 0);
			}
			ans = min(ans, z);
			for (int j = 2; j < n; j++)
				make(j, 1, -1);
			//cout << i << ' ' << z << endl;
		}
		cout << ans << endl;
	}

	re 0;
}
