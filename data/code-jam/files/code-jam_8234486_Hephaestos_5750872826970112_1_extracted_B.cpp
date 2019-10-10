#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <limits>
#include <deque>
#include <cstdint>

using namespace std;


void main() {
	int T;
	cin >> T;
	for (int i = 0; i < T; ++i) {
		int r=0;
		int n;
		double v, x;
		cin >> n >> v >> x;
		vector<pair<double, double>> sources;
		double flow = 0;
		double heat = 0;
		for (int j = 0;j < n;++j) {
			double f, t;
			cin >> f >> t;
			sources.push_back(make_pair(t - x, f));
			flow += f;
			heat += f*(t - x);
		}
		sort(sources.begin(), sources.end());
		bool possible = true;
		while (heat > 1e-9) {
			if (sources[0].first > 0) {
				possible = false;
				break;
			}
			for (int i = n - 1;i >= 0;--i) {
				double h = sources[i].first;
				if (h<0) {
					possible = false;
					break;
				}
				if (heat - h*sources[i].second >= 1e-9) {
					flow -= sources[i].second;
					heat -= h*sources[i].second;
				}
				else {
					flow -= heat / h;
					heat = 0;
					break;
				}
			}
		}
		while (heat < -1e-9) {
			if (sources.back().first < 0) {
				possible = false;
				break;
			}
			for (int i = 0;i < n;++i) {
				double h = sources[i].first;
				if (h>0) {
					possible = false;
					break;
				}
				if (abs(heat) - abs(h*sources[i].second) >=1e-9 ) {
					flow -= sources[i].second;
					heat -= h*sources[i].second;
				}
				else {
					flow -= heat / h;
					heat = 0;
					break;
				}
			}
		}
		cout.precision(9);
		if (possible) {
			cout << "Case #" << i + 1 << ": " << v/flow << endl;
		}
		else {
			cout << "Case #" << i + 1 << ": " << "IMPOSSIBLE" << endl;
		}
	}
}