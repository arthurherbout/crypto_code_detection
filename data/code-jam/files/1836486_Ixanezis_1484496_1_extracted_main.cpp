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

int n;

long long s[555];

struct number {
	long long val;
	int mask;

	number() : val(0), mask(0) {}


	bool operator < (const number &rhs) const {
		return val < rhs.val;
	}
};

number *a;
number *b;

int main() {
	freopen("C-large.in", "r", stdin);
	freopen("output.out", "w", stdout);

	int t; cin >> t;

	const int LIM = 23;
	a = new number[(1 << LIM) + 1];
	b = new number[(1 << LIM) + 1];

	for (int e=1; e<=t; e++) {
		cin >> n;

		for (int i=0; i<n; i++) cin >> s[i];

		cout << "Case #" << e << ":\n";

		int ea = 0;
		int eb = 0;
		for (int mask = 1; mask < (1 << LIM); mask ++) {
			long long cur = 0;
			for (int i=0; i<LIM; i++) {
				if (mask & (1 << i)) {
					cur += s[i];
				}
			}
			a[ea].mask = mask;
			a[ea].val = cur;
			ea++;
		}

		for (int mask = 1; mask < (1 << LIM); mask ++) {
			long long cur = 0;
			for (int i=0; i<LIM; i++) {
				if (mask & (1 << i)) {
					cur += s[i + 33];
				}
			}
			b[eb].mask = mask;
			b[eb].val = cur;
			eb++;
		}

		sort(a, a+ea);
		sort(b, b+eb);

		int i=0, j=0;
		bool can = false;
		while (i < ea && j < eb) {
			while (a[i] < b[j] && i < ea) ++i;
			while (b[j] < a[i] && j < eb) ++j;
			if (i == ea || j == eb) break;
			if (a[i].val == b[j].val) {
				for (int t=0; t<LIM; t++) {
					if (a[i].mask & (1 << t)) {
						cout << s[t] << ' ';
					}
				}
				cout << endl;
				for (int t=0; t<LIM; t++) {
					if (b[j].mask & (1 << t)) {
						cout << s[t+33] << ' ';
					}
				}
				cout << endl;
				can = true;
				break;
			}
		}

		if (!can) {
			cout << "Oh no no no" << endl;
		}
	}

	return 0;
}
