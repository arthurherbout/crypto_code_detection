#include <cstdio>
#include <vector>
#include <cmath>
#include <set>
#include <map>
#include <algorithm>
#include <cstring>
#include <string>
#include <iostream>
#include <cassert>
#include <memory.h>
using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)
#define foreach(it, a) for (__typeof((a).begin()) it = (a).begin(); it != (a).end(); it++)
#define pb push_back
typedef pair<int, int> pii;
typedef long long ll;
typedef long double ld;

const long long kInf = 1000000000000000000LL;

long long a, b, n;
map<long long, long long> M;

long long Cost(long long x, int depth = 0) {
	if (M.find(x) != M.end()) {
		return M[x];
	}
	if (x == 1) {
		return 0;
	}
	else if (x == 2) {
		return a;
	}
	long long ret = kInf;
	if (true || n < 100000000) {
		long long lb = max(1LL, x / 2 - 100), ub = x / 2;
		for (long long i = lb; i <= ub; ++i) {
			ret = min(ret, max(Cost(i, depth + 1) + b, Cost(x - i, depth + 1) + a));
		}
	}
	else {
		ret = Cost(x / 2, depth + 1) + b;
	}
	return M[x] = ret;
}

void Solve() {
	M.clear();
	cin >> n >> a >> b;
	cout << Cost(n) << endl;
}

int main() {
	freopen("e.in", "r", stdin);
	freopen("e.out", "w", stdout);
	int tc = 0;
	int T;
	scanf("%d", &T);
	while (T--) {
		printf("Case #%d: ", ++tc);
		Solve();
	}
	return 0;
}