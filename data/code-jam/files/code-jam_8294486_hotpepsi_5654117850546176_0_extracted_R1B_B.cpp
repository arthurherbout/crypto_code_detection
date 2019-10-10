// Google Code Jam 2017 Round 1B
// Problem B.

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

typedef pair<int, int> II;
typedef vector<II> IIVec;

string solve() {
	int r, o, y, g, b, v, n;
	cin >> n >> r >> o >> y >> g >> b >> v;
	if (r > n / 2 || y > n / 2 || b > n / 2) {
		return "IMPOSSIBLE";
	}
	IIVec x;
	x.push_back(II(r, 'R'));
	x.push_back(II(y, 'Y'));
	x.push_back(II(b, 'B'));
	sort(x.rbegin(), x.rend());
	char w[1024] = {};
	for (int i = 0; i < 2; ++i) {
		int m = (n / 2) + (i == 0 && (n % 2) != 0);
		for (int j = 0; j < m; ++j) {
			for (int k = 0; k < 3; ++k) {
				if (x[k].first) {
					--x[k].first;
					w[i + j * 2] = x[k].second;
					break;
				}
			}
		}
	}
	return w;
}

int main(int argc, char *argv[]) {
	int T;
	cin >> T;
	for (int t = 1; t <= T; ++t) {
		string ans = solve();
		cout << "Case #" << t << ": " << ans << endl;
	}
	return 0;
}
