#include <iostream>
#include <cmath>
#include <list>
using namespace std;

typedef signed long long longf;
typedef unsigned long long ulongf;
        // r  c
char map[30][30];
int r, c;

int solve() {
	cin >> r >> c;

	for (int i = 0; i < r; ++i) {
		cin >> map[i];
	}

	for (int rr = 0; rr < r; ++rr) {
		char pre = 0;
		for (int cc = 0; cc < c; ++cc) {
			if (map[rr][cc] != '?') {
				pre = map[rr][cc];
			} else{
				if (pre != 0)
					map[rr][cc] = pre;
			}
		}
	}

	for (int rr = 0; rr < r; ++rr) {
		char pre = 0;
		for (int cc = c - 1; cc >= 0; --cc) {
			if (map[rr][cc] != '?') {
				pre = map[rr][cc];
			} else{
				if (pre != 0)
					map[rr][cc] = pre;
			}
		}
	}

	for (int cc = 0; cc < c; ++cc) {
		char pre = 0;
		for (int rr = 0; rr < r; ++rr) {
			if (map[rr][cc] != '?') {
				pre = map[rr][cc];
			} else{
				if (pre != 0)
					map[rr][cc] = pre;
			}
		}
	}

	for (int cc = 0; cc < c; ++cc) {
		char pre = 0;
		for (int rr = r - 1; rr >= 0; --rr) {
			if (map[rr][cc] != '?') {
				pre = map[rr][cc];
			} else{
				if (pre != 0)
					map[rr][cc] = pre;
			}
		}
	}

	for (int i = 0; i < r; ++i) {
		cout << map[i] << endl;
	}

	return 1;
}

int t;

int main() {
	cin >> t;

	for (int i = 1; i <= t; ++i) {
		cout << "Case #" << i << ":" << endl;
		solve();
	}

	return 1;
}