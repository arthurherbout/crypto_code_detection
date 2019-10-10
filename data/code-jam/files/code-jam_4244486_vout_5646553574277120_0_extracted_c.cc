#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

int c, d, v;
int dd[10];
bool valid[33];

void dfs(int i, int vv) {
	if (i == d) {
		if (vv <= v) valid[vv] = true;
		return;
	}
	dfs(i + 1, vv);
	dfs(i + 1, vv + dd[i]);
}

int main() {
	int T;
	cin >> T;
	for (int re = 1; re <= T; ++re) {
		cin >> c >> d >> v;
		cerr << c << endl;
		for (int i = 0; i < d; ++i) {
			cin >> dd[i];
		}

		memset(valid, 0, sizeof(valid));
		dfs(0, 0);

		int ans = 0;
		for (int i = 1; i <= v; ++i) {
			if (!valid[i]) {
				++ans;
				for (int j = max(1, v - i); j >= 1; --j) {
					if (valid[j]) {
						valid[j + i] = true;
					}
				}
				valid[i] = true;
			}
		}

		cout << "Case #" << re << ": " << ans << endl;
	}
	return 0;
}