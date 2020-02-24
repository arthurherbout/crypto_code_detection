#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

const int MAXN = 10000 + 10, MAXM = 100 + 10, MAXL = 28;
string s[MAXN], t[MAXN];
char buf[10088];
int n, m;

int gao(int x, int y) {
	vector<int> can;
	for (int i = 0; i < n; ++i) {
		if (s[i].length() == s[x].length()) {
			can.push_back(i);
		}
	}

	int ret = 0;

	for (int i = 0; i < 26; ++i) {
		char c = t[y][i];
		bool ok = false;
		for (int j = 0; j < can.size(); ++j) {
			if (s[can[j]].find(c) != -1) {
				ok = true;
				break;
			}
		}

		if (!ok) {
			continue;
		}

		if (s[x].find(c) == -1) {
			++ret;
			vector<int> tmp;
			for (int j = 0; j < can.size(); ++j) {
				if (s[can[j]].find(c) == -1) {
					tmp.push_back(can[j]);
				}
			}
			can = tmp; 
		} else {
			vector<int> tmp;
			for (int j = 0; j < s[x].length(); ++j) {
				if (s[x][j] == c) {
					tmp.push_back(j);
				}
			}
			vector<int> t2;
			for (int j = 0; j < can.size(); ++j) {
				bool ok = true;
				for (int k = 0; k < tmp.size(); ++k) {
					if (s[can[j]][tmp[k]] != c) {
						ok = false;
						break;
					}
				}
				if (ok) {
					t2.push_back(can[j]);
				}
			}
			can = t2;
		}
	}
	return ret;
}

int main() {
	int T;
	scanf("%d", &T);
	for (int re = 1; re <= T; ++re) {
		scanf("%d%d", &n, &m);
		for (int i = 0; i < n; ++i) {
			scanf(" %s", buf);
			s[i] = buf;
		}

		printf("Case #%d: ", re);
		for (int i = 0; i < m; ++i) {
			scanf(" %s", buf);
			t[i] = buf;
			assert(t[i].length() == 26);
			int loss = -1, ans = 0;
			for (int j = 0; j < n; ++j) {
				int tmp = gao(j, i);
				if (loss < tmp) {
					loss = tmp;
					ans = j;
				}
			}
			printf("%s%c", s[ans].c_str(), i == m - 1 ? '\n' : ' ');
		}
	}
	return 0;
}
