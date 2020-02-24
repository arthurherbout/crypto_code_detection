#include <iostream>
#include <vector>
#include <map>

using namespace std;

const int N = 21;

vector<int> dp[1 << N];

int okey[N];
vector<int> keys[N];


int main() {
	int Tc;
	cin >> Tc;
	for (int re = 1; re <= Tc; ++re) {
		int k, n, key, x, y;
		map<int, int> curKeys, initKeys;
		cin >> k >> n;
		for (int i = 0; i < n; ++i) {
			keys[i].clear();
		}

		for (int i = 0; i < k; ++i) {
			cin >> key;
			initKeys[key]++;
		}
		for (int i = 0; i < n; ++i) {
			cin >> okey[i];
			cin >> x;
			for (int j = 0; j < x; ++j) {
				cin >> y;
				keys[i].push_back(y);
			}
		}

		for (int i = 0; i < (1 << n); ++i) {
			dp[i].clear();
		}

		for (int i = 0; i < (1 << n); ++i) {
			if (i != 0 && dp[i].size() == 0) {
				continue;
			}

			curKeys = initKeys;
			for (int j = 0; j < n; ++j) {
				if (i & (1 << j)) {
					for (int x = 0; x < keys[j].size(); ++x) {
						curKeys[keys[j][x]]++;	
					}
					curKeys[okey[j]]--;
				}
			}

			for (int j = 0; j < n; ++j) {
				if ((i & (1 << j)) || curKeys[okey[j]] < 1) {
					continue;
				}
				dp[i].push_back(j + 1);
				if (dp[i | (1 << j)].size() == 0 || dp[i | (1 << j)] > dp[i]) {
					dp[i | (1 << j)] = dp[i];
				}
				dp[i].pop_back();
			}
		}
		cout << "Case #" << re << ":";
		
		if (dp[(1 << n) - 1].size() == 0) {
			cout << " IMPOSSIBLE";
		} else {
			for (int i = 0; i < n; ++i) {
				cout << ' ' << dp[(1 << n) - 1][i];
			}
		}

		cout << endl;
	}
	return 0;
}
