#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 1024;
const int MAXL = 100000 * 1000 + 1000000 + 100;

typedef pair<pair<int, int>, pair<int, int> > Attack;

/*
struct SegTree {
	int _l, _r;
	int _cnt;

	int a[MAXL * 8];
	int l[MAXL * 8];
	int r[MAXL * 8];
	int lc[MAXL * 8];
	int rc[MAXL * 8];
	bool p[MAXL * 8];

	void _init(int id, int left, int right) {
		l[id] = left;
		r[id] = right;
		p[id] = false;
		if (left != right) {
			int mid = (left + right) / 2;
			lc[id] = ++_cnt;
			_init(_cnt, left, mid);
			rc[id] = ++_cnt;
			_init(_cnt, mid + 1, right);
		}
	}

	void init(int left, int right) {
		_l = left;
		_r = right;
		_cnt = 0;
		memset(a, 0, sizeof(a));
		_init(0, left, right);
	}

	int _query(int id, int left, int right) {
		if (left <= l[id] && right >= r[id]) {
			return a[id];
		}

		_push_down(id);

		int ret = 0;

		int mid = (l[id] + r[id]) / 2;

		if (left < mid) {
			ret = min(ret, _query(lc[id], left, right));
		}

		if (right > mid) {
			ret = min(ret, _query(rc[id], left, right));
		}
		
		return ret;
	}

	void _update(int id, int left, int right, int v) {
		
	}
};
*/

int a[MAXL * 4];

int main() {
	int Tc;
	cin >> Tc;
	int n, d, m, w, e, s, dd, dp, ds;
	vector<Attack> va;

	for (int re = 1; re <= Tc; ++re) {
		cin >> n;
		va.clear();
		for (int i = 0; i < n; ++i) {
			cin >> d >> m >> w >> e
				>> s >> dd >> dp >> ds;
			for (int j = 0; j < m; ++j) {
				va.push_back(make_pair(make_pair(d, s), make_pair(w, e)));
				d += dd;
				s = max(0, s + ds);
				w += dp;
				e += dp;
			}
		}

		memset(a, 0, sizeof(a));
		int ans = 0;
		sort(va.begin(), va.end());
		for (int i = 0, j; i < va.size(); i = j) {
			int curDay = va[i].first.first;
			for (j = i; j < va.size(); ++j) {
				if (va[j].first.first != curDay) {
					break;
				}
			}

			for (int k = i; k < j; ++k) {
				// count attack
				for (int x = va[k].second.first; x <= va[k].second.second; ++x) {
					if (a[(x + MAXL) * 2] < va[k].first.second || ( x != va[k].second.second && a[(x + MAXL) * 2 + 1] < va[k].first.second)) {
						++ans;
//					cout << "Success! " << curDay << '(' << va[k].second.first << ',' << va[k].second.second << ")\n";
						break;
					}
				}
			}

			for (int k = i; k < j; ++k) {
				// update
				for (int x = va[k].second.first; x <= va[k].second.second; ++x) {
					if (a[(x + MAXL) * 2] < va[k].first.second) {
						a[(x + MAXL) * 2] = va[k].first.second;
					}
					if (x != va[k].second.second && a[(x + MAXL) * 2 + 1] < va[k].first.second) {
						a[(x + MAXL) * 2 + 1] = va[k].first.second;
					}
				}
			}
		}

		cout << "Case #" << re << ": " << ans;

		cout << '\n';
	}
	return 0;
}
