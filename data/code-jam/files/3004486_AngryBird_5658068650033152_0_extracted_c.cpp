#include <utility>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int stones(int row, int col, int k) {
	if (k < 5)
		return k;
	if (row == 3) {
		if (k > row*col - 4) {
			return k-col+2;
		}
		int c = 2;
		k -= 2;
		c += k/3*2;
		c += k%3;
		return c;
	} else if (row == 4) {
		int c = row*col;
		if (k > row*col - 4) {
			return k - (col - 2) * 2;
		}
		else if (k <= row*col-col) {
			c = stones(row-1, col, k);
		}
		int t = 4;
		k -= 4;
		t += k/4*2;
		t += k%4;
		if (k%4 == 2)
			t -= 1;
		return t > c ? c : t;
	} else {
		return k;
	}
}

int main() {
	int T, N, M, K;
	cin >> T;
	for (int i = 0; i < T; i++) {
		cin >> N >> M >> K;
		cout << "Case #" << i+1 << ": ";
		if (N < M)
			cout << stones(N, M, K) << endl;
		else
			cout << stones(M, N, K) << endl;
	}
	return 0;
}