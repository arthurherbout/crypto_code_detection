#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <iostream>

using namespace std;

int T;
int N, M, K;
void solve() {
	cin >> N >> M >> K;
	if (N > M) swap(N, M);
	if (N <= 2 || K <= 4) {
		printf("%d\n", K);
		return;
	}
	if (N * M == K) {
		printf("%d\n", (N + M - 4) * 2 + 4);
		return;
	}
	int answer = 0x3fffffff;
	for (int i = 1; i < N - 1; ++i) {
		for (int j = 1; j < M - 1; ++j) {
			for (int k = 0; k <= 4; ++k) {
				if (2 * (i + j) + i * j + k == K) {
					answer = min(answer, 2 * (i + j) + k);
				}
			}
		}
	}
	printf("%d\n", answer);
}
int main() {
	// freopen("in", "r", stdin);
	freopen("C-small-attempt1.in", "r", stdin);
	freopen("C-small-attempt1.out", "w", stdout);
	cin >> T;
	for (int _ = 1; _ <= T; ++_) {
		printf("Case #%d: ", _);
		solve();
	}
	return 0;
}