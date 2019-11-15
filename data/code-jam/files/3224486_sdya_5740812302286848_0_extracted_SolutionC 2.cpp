#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <bitset>
#include <cmath>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

void readData() {
}

const int maxN = 1100;
int n, s;
int x[maxN], y[maxN], z[maxN];
int vx[maxN], vy[maxN], vz[maxN];

long long dist(int i, int j) {
	long long dx = x[i] - x[j];
	long long dy = y[i] - y[j];
	long long dz = z[i] - z[j];
	return dx * dx + dy * dy + dz * dz;
}

bool used[maxN];

void dfs(int v, long long d) {
	used[v] = true;
	for (int i = 0; i < n; ++i) {
		if (!used[i] && dist(v, i) <= d) {
			dfs(i, d);
		}
	}
}

void solve(int test) {
	cin >> n >> s;
	for (int i = 0; i < n; ++i) {
		cin >> x[i] >> y[i] >> z[i] >> vx[i] >> vy[i] >> vz[i];
	}

	long long left_bound = 0, right_bound = 1000000000000000000LL;
	while (right_bound - left_bound > 1) {
		long long middle = (left_bound + right_bound) / 2;
		memset(used, 0, sizeof(used));
		dfs(0, middle);
		if (used[1]) {
			right_bound = middle;
		} else {
			left_bound = middle;
		}
	}
	long long res = right_bound;
	memset(used, 0, sizeof(used));
	dfs(0, left_bound);
	if (used[1]) {
		res = left_bound;
	}
	printf("Case #%d: %.10lf\n", test, sqrt((double)(res)));
}

int main(int argc, char* argv[]) {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int left_bound, right_bound;

	/*freopen(argv[3], "w", stdout);	
	sscanf(argv[1], "%d", &left_bound);
	sscanf(argv[2], "%d", &right_bound);*/

	int t;
	cin >> t;
	left_bound = 1, right_bound = t;
	for (int i = 1; i <= t; ++i) {
		if (i >= left_bound && i <= right_bound) {
			solve(i);
		} else {
			readData();
		}
		cerr << i << ": " << clock() << endl;
	}

	return 0;
}
