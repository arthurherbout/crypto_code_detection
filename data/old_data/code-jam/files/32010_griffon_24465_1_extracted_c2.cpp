#pragma comment(linker,"/STACK:256000000")

#ifdef __GNUC__
#define int64 long long
#else /* MSVC, say */
#define int64 __int64
#endif 

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <ctype.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <stack>

using namespace std;

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

string taskname = "c2";

#define MAXN 512
#define MOD 1000000009

vector<int> a[MAXN];
int n;
int64 k;

int64 mypow(int n, int k) {
	int64 ret = 1;
	int64 curp = n;
	while (k) {
		if (k & 1) {
			ret *= curp;
			ret %= MOD;
		}
		curp *= curp;
		curp %= MOD;
		k >>= 1;
	}
	return ret;
}

int64 inv(int k) {
	return mypow(k, MOD - 2);
}

int64 cnk(int64 n, int64 k) {
	if (k > n || k < 0) {
		return 0;
	}
	int64 ret = 1;
	for (int i = n - k + 1; i <= n; i++) {
		ret *= i;
		ret %= MOD;
	}
/*	for (int i = 1; i <= k; i++) {
		ret *= inv(i);
		ret %= MOD;
	}
*/	return ret;
}

int64 get(int v, int prev) {
	vector<int> c;
	for (int i = 0; i < a[v].size(); i++) {
		if (a[v][i] != prev) {
			c.push_back(a[v][i]);
		}
	}
	int64 ret = cnk(k - a[prev].size(), c.size());
//	cerr << "V = "<< v << " " << " CHILDREN = " << c.size() << endl;
//	cerr << "COLORS = " << k - a[prev].size() << " K = " << c.size() << endl;
//	cerr << "RET = " << ret << endl;
	for (int i = 0; i < c.size(); i++) {
		ret *= get(c[i], v);
		ret %= MOD;
	}
	
	return ret;
}

int main() {
	freopen((taskname + ".in").c_str(), "r", stdin);
	freopen((taskname + ".out").c_str(), "w", stdout);

	int tests;
	scanf("%d", &tests);
	for (int test = 0; test < tests; test++) {
		printf("Case #%d: ", test + 1);
		scanf("%d %lld", &n, &k);			
		for (int i = 0; i <= n; i++) {
			a[i].clear();
		}
		for (int i = 0; i < n - 1; i++) {
			int v1, v2;
			scanf("%d %d", &v1, &v2);
			a[v1].push_back(v2);
			a[v2].push_back(v1);
		}
		printf("%lld", get(1, 0));

		printf("\n");
	}

	return 0;
}
