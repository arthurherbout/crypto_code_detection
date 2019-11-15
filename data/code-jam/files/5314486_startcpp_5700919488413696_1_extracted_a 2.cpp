//今までに食べたチョコレートの個数 % P != 0のときの来客数を最小化
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;

int t;
int n, p;
int g[100];
int dp[101][101][101][101];

int dfs(vector<int> rem, int cnt, int mod) {
	if (cnt == 0) { return 0; }
	if (dp[rem[0]][rem[1]][rem[2]][rem[3]] != -1) return dp[rem[0]][rem[1]][rem[2]][rem[3]];
	
	int ret = 114514;
	for (int i = 0; i < p; i++) {
		if (rem[i] > 0) {
			rem[i]--;
			int cst = (mod != 0) + dfs(rem, cnt - 1, (mod + i) % p);
			rem[i]++;
			ret = min(ret, cst);
		}
	}
	return dp[rem[0]][rem[1]][rem[2]][rem[3]] = ret;
}

int main() {
	cin >> t;
	for (int id = 1; id <= t; id++) {
		cin >> n >> p;
		
		int num[4] = {0};
		for (int i = 0; i < n; i++) { cin >> g[i]; num[g[i] % p]++; }
		
		for (int i = 0; i <= num[0]; i++) {
			for (int j = 0; j <= num[1]; j++) {
				for (int k = 0; k <= num[2]; k++) {
					for (int l = 0; l <= num[3]; l++) {
						dp[i][j][k][l] = -1;
					}
				}
			}
		}
		
		int res = dfs({num[0], num[1], num[2], num[3]}, n, 0);
		cout << "Case #" << id << ": " << n - res << endl;
	}
	return 0;
}