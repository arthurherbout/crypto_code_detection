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

string taskname = "d1";

#define MOD 30031
#define MAXP (1 << 11)
#define MAXN (1 << 10)

int mem[MAXP][MAXN];
int p, n, k;
int startmask;

int get(int mask, int left) {
	int & ret = mem[mask][left];
	if (ret != -1) {
		return ret;
	}
	
	if (mask >= (1 << p)) {
		ret = 0;
		return ret;
	} else if (left == 0) {
		if (mask == startmask) {
			ret = 1;
		} else {
			ret = 0;
		}
		return ret;
	}
	ret = 0;
	for (int i = 0; i < p; i++) {
		if (mask & (1 << i)) {
			int nextmask = mask & (~(1 << i));
			nextmask <<= 1;
			nextmask |= 1;
			ret += get(nextmask, left - 1);	
			ret %= MOD;
		}
	}
	return ret;
}

int main() {
	freopen((taskname + ".in").c_str(), "r", stdin);
	freopen((taskname + ".out").c_str(), "w", stdout);

	int tests;
	scanf("%d", &tests);
	for (int test = 0; test < tests; test++) {
		memset(mem, -1, sizeof(mem));
		printf("Case #%d: ", test + 1);
		scanf("%d %d %d", &n, &k, &p);
		startmask = 0;
		for (int i = 0; i < k; i++) {
			startmask |= (1 << i);
		}
		cerr << startmask << " " << n - k << endl;
		printf("%d", get(startmask, n - k));
		printf("\n");
	}

	return 0;
}
