#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

int n, m;
string zipcode[111];
bool outwent[111];
bool visited[111];
vector<int> edges[111];
bool can_return[111][111];
string ans;
string cur_ans;

void gao(int cur) {
	bool all_visited = true;
	for (int i = 0; i < n; ++i) {
		if (!visited[i]) {
			all_visited = false;
			break;
		}
	}
	if (all_visited) {
		if (ans > cur_ans) {
			ans = cur_ans;
		}
		return;
	}

	for (auto it = edges[cur].begin(); it != edges[cur].end(); ++it) {
		if (can_return[cur][*it]) {
			can_return[cur][*it] = false;
			gao(*it);
			// bt
			can_return[cur][*it] = true;
		}
	}

	for (auto it = edges[cur].begin(); it != edges[cur].end(); ++it) {
		if (!outwent[*it]) {
			outwent[*it] = true;
			can_return[*it][cur] = true;
			if (!visited[*it]) {
				visited[*it] = true;
				cur_ans += zipcode[*it];
				gao(*it);
				cur_ans = cur_ans.substr(0, cur_ans.length() - 5);
				visited[*it] = false;
			} else {
				gao(*it);
			}
			can_return[*it][cur] = false;
			outwent[*it] = false;
		}
	}
}

int main() {
	int T;
	cin >> T;
	int a, b;
	for (int re = 1; re <= T; ++re) {
		cin >> n >> m;
		for (int i = 0; i < n; ++i) {
			cin >> zipcode[i];
			edges[i].clear();
			fill(can_return[i], can_return[i] + n, false);
		}
		fill(outwent, outwent + n, false);
		fill(visited, visited + n, false);

		for (int i = 0; i < m; ++i) {
			cin >> a >> b;
			edges[a - 1].push_back(b - 1);
			edges[b - 1].push_back(a - 1);
		}

		ans = ":";
		for (int i = 0; i < n; ++i) {
			cur_ans = zipcode[i];
			visited[i] = true;
			outwent[i] = true;
			gao(i);
			outwent[i] = false;
			visited[i] = false;
		}
		cout << "Case #" << re << ": " << ans << endl;
	}
	return 0;
}