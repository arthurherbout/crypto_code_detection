#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

int a,b;
int s[555];
int n;

int A[555], B[555];
int aa,bb;
bool can;

void DFS(int pos) {
	if (a == b && a) {
		can = true;
		throw 5;
	}

	if (pos == n) return;

	a += s[pos];
	A[aa++] = s[pos];
	DFS(pos + 1);
	--aa;
	a -= s[pos];

	b += s[pos];
	B[bb++] = s[pos];
	DFS(pos + 1);
	--bb;
	b -= s[pos];

	DFS(pos + 1);
}

int main() {
	freopen("C-small.in", "r", stdin);
	freopen("output.out", "w", stdout);

	int t; cin >> t;

	for (int e=1; e<=t; e++) {
		cin >> n;

		for (int i=0; i<n; i++) cin >> s[i];

		cout << "Case #" << e << ":\n";

		int sum = std::accumulate(s, s+n, 0);
		can = false;

		a = b = 0;
		aa = bb = 0;
		try {
			DFS(0);
		} catch (...) {}

		if (!can) {
			cout << "Impossible" << endl;
		} else {
			for (int i=0; i<aa; i++)
				cout << A[i] << ' ';
			cout << endl;
			for (int i=0; i<bb; i++) {
				cout << B[i] << ' ';
			}
			cout << endl;
		}
	}

	return 0;
}
