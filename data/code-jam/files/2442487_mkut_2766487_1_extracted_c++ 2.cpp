#include <iostream>
using namespace std;

long long most(int N, long long x) {
	long long ret = 0;
	long long below = (1ull<<N) - x - 1;
	for (int i = 0; i < N; i++) {
		ret = below > 0 ? ret * 2 : ret * 2 + 1;
		below = (below - 1) / 2;
	}
	return ret;
}

long long least(int N, long long x) {
	long long ret = 0;
	long long upper = x;
	for (int i = 0; i < N; i++) {
		ret = upper > 0 ? ret * 2 + 1 : ret * 2;
		upper = (upper - 1) / 2;
	}
	return ret;
}

long long binary1(long long left, long long right, int N, long long P) {
	if (left >= right - 1) return left;
	long long mid = (left + right) / 2;
	if (least(N, mid) < P) {
		return binary1(mid, right, N, P);
	} else {
		return binary1(left, mid, N, P);
	}
}

long long binary2(long long left, long long right, int N, long long P) {
	if (left >= right - 1) return left;
	long long mid = (left + right) / 2;
	if (most(N, mid) < P) {
		return binary2(mid, right, N, P);
	} else {
		return binary2(left, mid, N, P);
	}
}

int main() {
	int T; cin >> T;
	for (int caseNo = 1; caseNo <= T; caseNo++) {
		long long N, P; cin >> N >> P;

		long long ans1 = binary1(0, 1ull<<N, N, P);
		long long ans2 = binary2(0, 1ull<<N, N, P);
		cout << "Case #" << caseNo << ": " << ans1 << " " << ans2 << endl;;
	}
	return 0;
}
