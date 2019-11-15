#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

vector<pair<pair<int, int>, int> > vp;

long long getCharge(int n, int x) {
	return 1LL * (n + 1 - x + n) * x / 2;
}

int MOD = 1000002013;

int main() {
	int T;
	cin >> T;
	for (int re = 1; re <= T; ++re) {
		int n, m, x, y, z;
		long long ans1 = 0;
		cin >> n >> m;
		vp.resize(m);
		for (int i = 0; i < m; ++i) {
			cin >> x >> y >> z;
			vp[i] = make_pair(make_pair(x, y), z);
		}

		long long loss = 1;
		long long ans = 0;
		
		while (loss != 0) {
			int x = -1, y = -1;
			loss = 0;
			m = vp.size();
			for (int i = 0; i < m; ++i) {
				if (vp[i].second == 0) continue;
				for (int j = 0; j < m; ++j) {
					if (vp[j].second == 0) continue;
					if ((vp[i].first.first < vp[j].first.first && vp[i].first.second >= vp[j].first.first && vp[j].first.second > vp[i].first.second)) {
						int ex = min(vp[i].second, vp[j].second);
						long long _loss = getCharge(n, vp[i].first.second - vp[i].first.first) \
										 + getCharge(n, vp[j].first.second - vp[j].first.first) \
										 - getCharge(n, vp[j].first.second - vp[i].first.first) \
										 - getCharge(n, vp[i].first.second - vp[j].first.first);
						if (_loss > loss) {
							x = i, y = j;
							loss = _loss;
						}
					}
				}
			}

			if (loss > 0) {
				int ex = min(vp[x].second, vp[y].second);
				vp[x].second -= ex;
				vp[y].second -= ex;
				vp.push_back(make_pair(make_pair(vp[x].first.first, vp[y].first.second), ex));
				vp.push_back(make_pair(make_pair(vp[y].first.first, vp[x].first.second), ex));	
				ans += loss * ex % MOD;
				ans %= MOD;
			}
		}

		cout << "Case #" << re << ": " << ans << endl;
	}
	return 0;
}
