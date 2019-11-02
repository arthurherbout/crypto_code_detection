#include <iostream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <bitset>
#include <deque>
#include <map>
#include <stack>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#define mp make_pair
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int)((x).size()))
#define sqr(x) ((x)*(x))
#define For(i,a,b) for(int i = (a); i < (b); i++)
#define rep(i,n) For(i,0,n)
#define rrep(i,n) for (int i = ((n) - 1); i >= 0; i--)
#define re return
#define fi first
#define se second
#define y0 y47847892
#define y1 y47824262
#define fill(x, val) memset(x, val, sizeof(x))

using namespace std;

typedef vector<int> vi;
typedef vector<string> vs;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<vi> vvi;
typedef long long ll;

template<class T> T abs(T x) { return x > 0 ? x : -x;}

int n;
int m;

int col[3000000];
int ans;
int was[3000000];

queue<int> q;

void add(int x) {
	if (col[x] >= 2 && !was[x]) {
		was[x] = 1;
		q.push(x);
	}
}

int main() {

#ifndef ONLINE_JUDGE
	freopen("test.in", "r", stdin);
	freopen("test.out", "w", stdout);
#endif

	int tc;
	cin >> tc;

	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		fill(col, 0);
		cin >> n;
		rep(i, n) {
			int a, b;
			cin >> a >> b;
			a += 1500000;
			col[a] += b;
			add(a);
		}
		ans = 0;

		while (!q.empty()) {
			int x = q.front();
			q.pop();
			was[x] = 0;
			int u = col[x];
			//cout << x << ' ' << u << endl;
			ans += u / 2;
			col[x] %= 2;
			col[x - 1] += u / 2;
			add(x - 1);
			col[x + 1] += u / 2;
			add(x + 1);
		}

		cout << ans << endl;
	}

	return 0;
}
