#include <iostream>
#include <vector>
#include <set>
using namespace std;

int main() {
	int T; cin >> T;
	for (int No = 1; No <= T; No++) {
		int N; cin >> N;
		vector<int> vi(N - 1);
		for (int i = 0; i < N - 1; i++) {
			cin >> vi[i]; vi[i]--;
		}

		set<int> convex;
		vector<int> ans(N);
		ans[N-1] = ans[N-2] = 0;
		convex.insert(N-1);
		convex.insert(N-2);
		bool ok = true;
		for (int i = N-3; i >= 0; i--) {
			set<int>::iterator it = convex.lower_bound(vi[i]);
			if (it == convex.end() || *it != vi[i]) {
				ok = false;
				break;
			}
			if (it == convex.begin()) {
				int b = *it;
				it++;
				int c = *it;
				it--;
				ans[i] = ans[b] - (ans[c] - ans[b]) * (b - i) / (c - b) - 1;
				convex.erase(convex.begin(), it);
				convex.insert(i);
			} else if (*it == N-1) {
				int b = *it;
				it--;
				int a = *it;
				it++;
				ans[i] = ans[a] - (ans[b] - ans[a]) * (a - i) / (b - a) + 1;
				convex.erase(convex.begin(), it);
				convex.insert(i);
			} else {
redo:
				it--;
				int a = *it;
				it++;
				int b = *it;
				it++;
				int c = *it;
				it--;
				int x = ans[b] - (ans[c] - ans[b]) * (b - i) / (c - b) - 1;
				int y = ans[a] - (ans[b] - ans[a]) * (a - i) / (b - a) + 1;
				if (y > x) {
					for (int k = 0; k < N; k++) {
						ans[k] *= 3;
					}
					goto redo;
				}
				ans[i] = (x + y) / 2;
				convex.erase(convex.begin(), it);
				convex.insert(i);
			}
		}
		cout << "Case #" << No << ": ";
		if (ok) {
			int mini = 0;
			for (int i = 0; i < N; i++) {
				mini = min(mini, ans[i]);
			}
			for (int i = 0; i < N; i++) {
				cout << (i == 0 ? "" : " ") << ans[i] - mini + 1;
				//cout << (i == 0 ? "" : " ") << ans[i];
			}
			cout << endl;
		} else {
			cout << "Impossible" << endl;
		}
	}
	return 0;
}
