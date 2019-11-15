//今までに食べたチョコレートの個数 % P != 0のときの来客数を最小化
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;

int t;
int n, p;
int g[100];
map<vector<int>, int> dp;

int dfs(vector<int> rem, int cnt, int mod) {
	if (cnt == 0) { return 0; }
	if (dp.find(rem) != dp.end()) return dp[rem];
	
	int ret = 114514;
	for (int i = 0; i < p; i++) {
		if (rem[i] > 0) {
			rem[i]--;
			int cst = (mod != 0) + dfs(rem, cnt - 1, (mod + i) % p);
			rem[i]++;
			ret = min(ret, cst);
		}
	}
	
	//cout << "dp[";
	//for (int i = 0; i < 4; i++) cout << rem[i] << " ";
	//cout << "] = " << ret << endl;
	
	return dp[rem] = ret;
}

int main() {
	cin >> t;
	for (int id = 1; id <= t; id++) {
		cin >> n >> p;
		
		int num[4] = {0};
		for (int i = 0; i < n; i++) { cin >> g[i]; num[g[i] % p]++; }
		dp.clear();
		int res = dfs({num[0], num[1], num[2], num[3]}, n, 0);
		cout << "Case #" << id << ": " << n - res << endl;
	}
	return 0;
}