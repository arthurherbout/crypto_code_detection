#include <bits/stdc++.h>
using namespace std;

int T, N, Q, D[128][128], E[128], S[128], U, V;

double dp[128];

int main() {
	cin >> T;
	for (int t=1; t<=T; t++) {
		cin >> N >> Q;
		for (int i=1; i<=N; i++)
			cin >> E[i] >> S[i];
		for (int i=1; i<=N; i++)
			for (int j=1; j<=N; j++)
				cin >> D[i][j];

		cout << "Case #" << t << ": ";

		for (int q=0; q<Q; q++) {
			cin >> U >> V;
			
			dp[1] = 0;
			for (int i = 2; i <= N; i++)
				dp[i] = 1E50;

			for (int i = 1; i <= N; i++) {
				int dist = 0;
				for (int j = i+1; j <= N; j++) {
					dist += D[j-1][j];
					if (dist <= E[i]) {
						double new_reach = dp[i] + ((double)dist) / S[i];
						if (new_reach < dp[j])
							dp[j] = new_reach;
					}
				}
			}

			printf("%.9f ", dp[N]);
		}
		cout << endl;
	}
}
