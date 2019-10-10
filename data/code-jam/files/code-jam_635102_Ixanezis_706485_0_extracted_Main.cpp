#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cctype>

using namespace std;

const int INF = 0x7f7f7f7f;

inline int prev(int x) {
	return (x & (x-1));
}

inline int next (int x) {
	return (x << 1) - (x & (x-1));
}

struct ddd {
	int x;
	int y;

	bool operator < (const ddd &ot) const {
		return x < ot.x || (x == ot.x && y < ot.y);
	}
};

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int t; cin >> t;

	for (int e=1; e<=t; e++) {
		int ans = 0;
		set<ddd> l;
		int r; cin >> r;
		for (int i=0; i<r; i++) {
			int x1, y1, x2, y2;
			cin >> x1 >> y1 >> x2 >> y2;
			for (int x=x1; x<=x2; ++x) {
				for (int y=y1; y<=y2; ++y) {
					ddd tmp = {x,y};
					l.insert(tmp);
				}
			}
		}

		set <ddd> l2;
		for (; ; ans++) {
		//	map<ddd, bool> m;

			l2.clear();
			for (set<ddd>::iterator it = l.begin(); it != l.end(); ++it) {

				ddd up = {it->x-1, it->y};
				ddd left = {it->x, it->y-1};
				if (l.find(up) != l.end() || l.find(left) != l.end() ) {
					l2.insert(*it);
				}

				for (int i=0; i<4; i++) {
					int x = it->x + dx[i];
					int y = it->y + dy[i];

					ddd up = {x, y-1};
					ddd left = {x-1, y};

					if (l.find(up) != l.end() && l.find(left) != l.end() ) {
						ddd ins = {x, y};
						l2.insert(ins);
					}
				}
			}

			if (l2.size() == 0) break;
			l = l2;
		}



		cout << "Case #" << e << ": " << ans+1 << endl;
	}

	return 0;
}

