#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <map>

using namespace std;

#define rep(i,n) for(int i = 0, _##i = n; i < _##i; ++i)
#define dwn(i,n) for(int i = n; i >= 0; --i)
#define itr(v, it) for(__typeof(v.begin()) it = v.begin(); it != v.end(); it++)

#define vi vector<int>
#define pb push_back
#define ll long long

#define min(a, b) (a<b?a:b)
#define max(a, b) (a>b?a:b)

#define INF (int) 1e9
#define EPS 1e-9

void solve() {

	int R, C, W;

	cin >> R >> C >> W;

	int result = (C/W) + (W-1) + (C%W?1:0);
	result += (R-1) * (C/W);

	printf("%d\n", result);
}

int main() {

	int cases;
	cin >> cases;

	rep(current, cases) {

		printf("Case #%d: ", current+1);
		solve();
	}

	return 0;
}