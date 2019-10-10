#define _CRT_SECURE_NO_WARNINGS

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
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

struct cell {
	int ceiling, floor;
	double time;
	bool foundBest;

	cell() : time(1e220), foundBest(false) {}

	bool isOk() const {
		return ceiling - floor >= 50;
	}
};

cell s[111][111];

int main() {
	freopen("B-small.in", "r", stdin);
	freopen("output.out", "w", stdout);

	int t; cin >> t;

	for (int e=1; e<=t; e++) {
		int h,n,m;
		cin >> h >> n >> m;

		for (int i=0; i<n; i++) {
			for (int u=0; u<m; u++) {
				s[i][u] = cell();
			}
		}

		for (int i=0; i<n; i++) {
			for (int u=0; u<m; u++) {
				cin >> s[i][u].ceiling;
			}
		}

		for (int i=0; i<n; i++) {
			for (int u=0; u<m; u++) {
				cin >> s[i][u].floor;
			}
		}

		set<pair<double, pair<int, int>>> ss;
		s[0][0].time = 0;
		ss.insert(make_pair(0.0, make_pair(0,0)));

		int dx[] = {-1,1,0,0};
		int dy[] = {0,0,-1,1};

		double ans;
		while (!ss.empty()) {
			pair<double, pair<int, int>> best = *ss.begin();
			ss.erase(ss.begin());

			if (best.second == make_pair(n-1, m-1)) {
				ans = best.first;
				break;
			}

			int x = best.second.first;
			int y = best.second.second;

			if (s[x][y].foundBest) continue;
			s[x][y].foundBest = true;

			for (int i=0; i<4; i++) {
				int nx = x + dx[i];
				int ny = y + dy[i];
				if (nx >= 0 && ny >= 0 && nx < n && ny < m) {
					if (s[nx][ny].isOk() && s[nx][ny].foundBest == false) {
						if (s[nx][ny].ceiling - s[x][y].floor >= 50) {
							if (s[x][y].ceiling - s[nx][ny].floor >= 50) {
								double curWaterLevel = h - 10.0 * best.first;
								double finalTime = best.first;
								
								if (s[nx][ny].ceiling - curWaterLevel < 49.999999999999) {
									finalTime += (50.0 - (s[nx][ny].ceiling - curWaterLevel)) / 10;
								}

								if (finalTime < 1e-9) {
								} else {
									curWaterLevel = h - 10.0 * finalTime;
									if (curWaterLevel - s[x][y].floor >= 19.9999999999999999) {
										finalTime += 1;
									} else {
										finalTime += 10;
									}
								}

								pair<double, pair<int, int>> toInsert = make_pair(finalTime, make_pair(nx, ny));
								ss.insert(toInsert);
							}
						}
					}
				}
			}
		}

		cout << "Case #" << e << ": " << fixed << setprecision(8) << ans << endl;
	}

	return 0;
}
