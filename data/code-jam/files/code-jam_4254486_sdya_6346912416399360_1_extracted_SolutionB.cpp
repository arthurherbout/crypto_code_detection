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

const int maxN = 1100;
int s[maxN], n, k;
int c[maxN];

int L[maxN], R[maxN];

void readData() {
}

bool check(int middle) {
	long long minS = 0, maxS = 0;
	for (int i = 0; i < k; ++i) {
		int mn = - L[i];
		int mx = middle - R[i];
		if (mn > mx) {
			return false;
		}
		minS += mn;
		maxS += mx;
	}
	for (long long sum = minS; sum <= maxS; ++sum) {
		if ((sum - s[k - 1]) % k == 0) {
			return true;
		}
	}
	return false;
}

void solve(int test) {
	cin >> n >> k;
	for (int i = 0; i < n; ++i) {
		s[i] = c[i] = 0;
	}
	for (int i = k - 1; i < n; ++i) {
		cin >> s[i];
	}
	for (int i = k; i < n; ++i) {
		c[i] = c[i - k] + s[i] - s[i - 1];
	}
	for (int i = 0; i < k; ++i) {
		L[i] = R[i] = 0;
	}
	for (int i = k; i < n; ++i) {
		L[i % k] = min(L[i % k], c[i]);
		R[i % k] = max(R[i % k], c[i]);
	}

	int left_bound = 0, right_bound = 1000000000;
	while (right_bound - left_bound > 1) {
		int middle = (left_bound + right_bound) / 2;
		if (check(middle)) {
			right_bound = middle;
		} else {
			left_bound = middle;
		}
	}

	int bound = right_bound;
	if (check(left_bound)) {
		bound = left_bound;
	}
	printf("Case #%d: %d\n", test, bound);
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
		}
		cerr << i << ": " << clock() << endl;
	}

	return 0;
}
