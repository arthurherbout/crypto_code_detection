#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <queue>
#include <cassert>
#include <set>

using namespace std;

double dp[200][200];
int tran[200][256];

int main() {
	int T;
	cin >> T;
	for (int re = 1; re <= T; ++re) {
		int nk, nl, ns;
		cin >> nk >> nl >> ns;
		string key, word;
		cin >> key >> word;

		memset(tran, 0, sizeof(tran));

		string s;
		for (int i = 0; i <= nl; ++i) {
			for (int j = 0; j < nk; ++j) {
				string next = s + key[j];
				for (int k = next.length(); k > 0; --k) {
					if (next.substr(next.length() - k) == word.substr(0, k)) {
						tran[i][j] = k;
						break;
					}
				}
			}
			if (i < nl) s.push_back(word[i]);
		}

		int shift = nl;
		assert(word.length() == nl);
		for (int i = 1; i < nl; ++i) {
			if ((word + word.substr(nl - i)).substr(i) == word) {
				shift = i;
				break;
			}
		}

		memset(dp, 0, sizeof(dp));
		dp[0][0] = 1;

		for (int i = 0; i < ns; ++i) {
			for (int j = 0; j <= nl; ++j) {
				for (int k = 0; k < nk; ++k) {
					dp[i + 1][tran[j][k]] += dp[i][j] / nk;
				}
			}
		}

		set<char> sk(key.begin(), key.end());
		double ans = (ns-nl)/shift + 1;
		if (ns < nl) ans = 0;
		for (int i = 0; i < nl; ++i) {
			if (sk.count(word[i]) == 0) ans = 0;
		}

		for (int i = 0; i <= ns; ++i) {
			if (dp[i][nl] > 0) {
				ans -= dp[i][nl];
			}
		}
		printf("Case #%d: %.10lf\n", re, ans);
	}
	return 0;
}