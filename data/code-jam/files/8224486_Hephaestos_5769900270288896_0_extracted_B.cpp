#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <limits>
#include <deque>
#include <cstdint>

using namespace std;


void main() {
	int T;
	cin >> T;
	for (int i = 0; i < T; ++i) {
		int r=0;
		int N, R, C;
		cin >> R >> C >> N;
		int n = N;
		int A = R*C;
		if (R == 1 || C == 1) {
			if (A % 2 == 0) {
				n -= A / 2;
				if (n > 0) {
					++r;
					--n;
				}
				for (int j = 0;j < max(n, 0);++j) {
					r += 2;
				}
			} else {
				n -= A / 2 + 1;
				for (int j = 0;j < max(n, 0);++j) {
					r += 2;
				}
			}
		} else if (A % 2 == 0) {
			n -= (A) / 2;
			for (int j = 0;j < max(0,min(n, 2));++j) {
				r += 2;
			}
			n -= 2;
			for (int j = 0;j < max(0, min(n, R + C - 4));++j) {
				r += 3;
			}
			n -= R + C - 4;
			for (int j = 0;j < max(0, n);++j) {
				r += 4;
			}
		}
		else {
			int r1 = 0;
			int n1 = n;
			n1 -= A / 2 + 1;
			for (int j = 0;j < max(0, min(n1, R + C - 2));++j) {
				r1 += 3;
			}
			n1 -= (R + C - 2);
			for (int j = 0;j < max(0, n1);++j) {
				r1+= 4;
			}
			int r2 = 0;
			int n2 = n - (A / 2);
			for (int j = 0;j < max(0, min(n2, 4));++j) {
				r2 += 2;
			}
			n2 -= 4;
			for (int j = 0;j < max(0, min(n2, R + C - 6));++j) {
				r2 += 3;
			}
			n2 -= R + C - 6;
			for (int j = 0;j < max(0, n2);++j) {
				r2 += 4;
			}
			r = min(r1, r2);
		}
		//lets try by occupying the corners
		n -= (R*C) / 2 + 1;
		int r1 = max(0, n);

		cout << "Case #" << i + 1 << ": " << r << endl;
	}
}