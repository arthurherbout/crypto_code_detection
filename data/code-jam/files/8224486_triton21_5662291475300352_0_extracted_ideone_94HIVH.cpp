#include <iostream>
using namespace std;
int N;
const int CIRC = 360;
long long H[2], D[2], M[2];
int main() {
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		cin >> N;
		for (int i = 0; i < N; i++) cin >> D[i] >> H[i] >> M[i];
		int ret = 1;
		if (N == 1 and H[0] == 1) {
			printf("Case #%d: 0\n", t);
			return 0;
		}
		if (N == 1) {
			M[1] = M[0] + 1, D[1] = D[0];
		}
		N = 2;
		D[0] = CIRC - D[0];
		D[1] = CIRC - D[1];
		if (M[0] < M[1]) {
			swap(D[0], D[1]);
			swap(M[0], M[1]);
		}
		int t1 = D[0] * M[0];
		int t2 = (D[1] + CIRC) * M[1];
		if (t2 <= t1) ret = 1;
		else ret = 0;
		printf("Case #%d: %d\n", t, ret);
	}
}