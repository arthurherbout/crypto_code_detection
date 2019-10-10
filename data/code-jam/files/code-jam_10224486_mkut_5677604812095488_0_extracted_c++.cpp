#include <iostream>
#include <vector>
#include <map>
using namespace std;

double solve(int k, vector<double> p) {
	int n = p.size();
	double ans = 0;
	for (long long i = 0; i < (1LL<<n); i++) {
		if (__builtin_popcount(i) != k) {
			continue;
		}
		vector<int> cand;
		for (int j = 0; j < n; j++) {
			if ((i >> j) & 1) {
				cand.push_back(j);
			}
		}
		vector<vector<double> > dp(k+1, vector<double>(k+1));
		dp[0][0] = 1;
		for (int a = 1; a <= k; a++) {
			for (int b = 0; b <= k; b++) {
				dp[a][b] = dp[a-1][b] * (1-p[cand[a - 1]]);
				if (b != 0) {
					dp[a][b] += dp[a-1][b-1] * p[cand[a - 1]];
				}
			}
		}
		ans = max(ans, dp[k][k/2]);
	}
	return ans;
}

int main() {
	int T; cin >> T;
	for (int No = 1; No <= T; No++) {
		int N, K; cin >> N >> K;
		vector<double> P(N);
		for (int i = 0; i < N; i++) {
			cin >> P[i];
		}
		double ans = solve(K, P);
		cout << "Case #" << No << ": " << ans << endl;
	}
	return 0;
}
