#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

int main() {
	int T; cin >> T;
	for (int No = 1; No <= T; No++) {
		int N, Q; cin >> N >> Q;
		vector<int> E(N), S(N), D(N-1);
		for (int i = 0; i < N; i++) {
			cin >> E[i] >> S[i];
		}
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int tmp; cin >> tmp;
				if (i + 1 == j) {
					D[i] = tmp;
				}
			}
		}
		int U, V; cin >> U >> V;
		vector<vector<double> > dp(N, vector<double>(N, 1e99));
		dp[0][0] = 0;
		for (int i = 0; i < N-1; i++) {
			for (int j = 0; j < N-1; j++) {
				int len = 0;
				for (int k = j; k <= i; k++) {
					len += D[k];
				}
				if (len <= E[j]) {
					dp[i+1][i+1] = min(dp[i+1][i+1], dp[i][j] + 1.0*D[i]/S[j]);
					dp[i+1][j] = min(dp[i+1][j], dp[i][j] + 1.0*D[i]/S[j]);
				}
			}
		}
		printf("Case #%d: %.8f\n", No, dp[N-1][N-1]);
	}
	return 0;
}
