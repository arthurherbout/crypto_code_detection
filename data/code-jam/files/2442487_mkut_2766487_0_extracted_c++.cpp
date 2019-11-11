#include <iostream>
using namespace std;

int most(int N, int x) {
	int ret = 0;
	int below = (1<<N) - x - 1;
	for (int i = 0; i < N; i++) {
		ret = below > 0 ? ret * 2 : ret * 2 + 1;
		below = (below - 1) / 2;
	}
	return ret;
}

int least(int N, int x) {
	int ret = 0;
	int upper = x;
	for (int i = 0; i < N; i++) {
		ret = upper > 0 ? ret * 2 + 1 : ret * 2;
		upper = (upper - 1) / 2;
	}
	return ret;
}

int binary1(int left, int right, int N, int P) {
	if (left >= right - 1) return left;
	int mid = (left + right) / 2;
	if (least(N, mid) < P) {
		return binary1(mid, right, N, P);
	} else {
		return binary1(left, mid, N, P);
	}
}

int binary2(int left, int right, int N, int P) {
	if (left >= right - 1) return left;
	int mid = (left + right) / 2;
	if (most(N, mid) < P) {
		return binary2(mid, right, N, P);
	} else {
		return binary2(left, mid, N, P);
	}
}

int main() {
	int T; cin >> T;
	for (int caseNo = 1; caseNo <= T; caseNo++) {
		int N, P; cin >> N >> P;

		int ans1 = binary1(0, 1<<N, N, P);
		int ans2 = binary2(0, 1<<N, N, P);
		cout << "Case #" << caseNo << ": " << ans1 << " " << ans2 << endl;;
	}
	return 0;
}
