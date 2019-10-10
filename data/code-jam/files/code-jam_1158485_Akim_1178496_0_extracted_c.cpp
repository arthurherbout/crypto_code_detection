#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>
#define pb push_back
#define mp make_pair
#define sz(v) ((int)(v).size())
#define all(v) (v).begin(),(v).end()

using namespace std;

typedef pair<int, int> ii;
typedef long long int64;
typedef vector<int> vi;

template<class T> T abs(T a) {return a > 0 ? a : (-a); }
template<class T> T sqr(T a) {return a * a; }

using namespace std;

int r, c;
vector<string> f;
bool a[10][10];
int cnt[10][10];

pair<int, int> get(int i, int j) {
	int di, dj;
	di = 0;
	dj = 0;
	if (f[i][j] == '|') {
		di = 1;
		dj = 0;
	}
	if (f[i][j] == '-') {
		di = 0;
		dj = 1;
	}
	if (f[i][j] == '/') {
		di = 1;
		dj = -1;
	}
	if (f[i][j] == '\\') {
		di = 1;
		dj = 1;
	}
	if (a[i][j]) {
		di *= -1;
		dj *= -1;
	}
	i += di;
	j += dj;
	if (i < 0) i += r;
	if (j < 0) j += c;
	if (i >= r) i -= r;
	if (j >= c) j -= c;
	return make_pair(i, j);
}

bool ok() {
	for (int i = 0; i < r; ++i)
		for (int j = 0; j < c; ++j) cnt[i][j] = 0;
	for (int i = 0; i < r; ++i)
		for (int j = 0; j < c; ++j) {
			pair<int, int> y = get(i, j);
			++cnt[y.first][y.second];
			if (cnt[y.first][y.second] > 1) return false;
		}

	return true;
			
}

void solve(int testnum) {
	cerr << testnum << endl;
	cin >> r >> c;
	f.resize(r);
	for (int i = 0; i < r; ++i) cin >> f[i];
	int ans = 0;
	for (int mask = 0; mask < (1 << (r*c)); ++mask) {
		for (int i = 0; i < r; ++i)
			for (int j = 0; j < c; ++j) {
				int t = i * c + j;
				a[i][j] = (mask & (1<<t)) == 0;
			}
		if (ok()) ++ans;
	}
	printf("Case #%d: %d\n", testnum, ans);
}

int main() {
	int t;
	scanf("%d", &t);
	for (int i = 1; i <= t; ++i) solve(i);
	return 0;
}
