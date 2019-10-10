#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <bitset>
#include <queue>
#include <cmath>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const long long P = 1000002013LL;
const long long I2 = 500001007LL;

const int maxM = 1100;

long long n, m;
long long o[maxM], e[maxM], p[maxM];
int g[maxM][maxM];
int used[maxM];

long long getCost(long long length) {
	if (length % 2 == 0) {
		return ((2 * n + 1 - length) * (length / 2)) % P;
	} else {
		return (((2 * n + 1 - length) / 2) * length) % P;
	}
}

bool comp(int x, int y) {
	return make_pair(o[x], e[x]) < make_pair(o[y], e[y]);
}

void solve(int test) {
	cin >> n >> m;
	long long total = 0;
	for (int i = 0; i < m; ++i) {
		cin >> o[i] >> e[i] >> p[i];
		total += getCost(e[i] - o[i]) * p[i];
		total %= P;
	}

	vector < int > order(m);
	for (int i = 0; i < m; ++i) {
		order[i] = i;
	}

	sort(order.begin(), order.end(), comp);

	vector < pair < long long, long long > > events;
	for (int i = 0; i < m; ++i) {
		events.push_back(make_pair(o[i], -p[i]));
		events.push_back(make_pair(e[i], p[i]));
	}
	
	sort(events.begin(), events.end());
	long long low_cost = 0;
	while (events.size() > 0) {
		vector < pair < long long, long long > > n_events;
		for (int i = 0; i + 1 < events.size(); ++i) {
			if (events[i].second < 0 && events[i + 1].second > 0) {
				long long d = min(events[i + 1].second, -events[i].second);
				low_cost += getCost(events[i + 1].first - events[i].first) * d;
				low_cost %= P;
				events[i + 1].second -= d;
				events[i].second += d;
				break;
			}
		}
		
		for (int i = 0; i < events.size(); ++i) {
			if (events[i].second != 0) {
				n_events.push_back(events[i]);
			}
		}
		events = n_events;
	}

	long long res = total - low_cost;
	res = ((res % P) + P) % P;
	cout << "Case #" << test << ": " << res << endl;

	/*memset(g, 0, sizeof(g));

	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= m; ++j) {
			if (max(o[i], o[j]) <= min(e[i], e[j])) {
				g[i][j] = true;
			}
		}
	}

	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= m; ++j) {
			for (int k = 1; k <= m; ++k) {
				if (g[j][i] && g[i][k]) {
					g[j][k] = true;
				}
			}
		}
	}

	long long trivial = 0;
	memset(used, 0, sizeof(used));
	for (int i = 1; i <= m; ++i) {
		if (used[i]) {
			continue;
		}
		vector < long long > a, b;
		for (int j = 1; j <= m; ++j) {
			if (g[i][j]) {
				used[j] = true;
				for (int k = 0; k < p[j]; ++k) {
					a.push_back(o[j]);
					b.push_back(e[j]);
				}
			}
		}
		sort(a.begin(), a.end());
		sort(b.rbegin(), b.rend());

		for (int j = 0; j < a.size(); ++j) {
			trivial += getCost(b[j] - a[j]);
			trivial %= P;
		}
	}

	memset(used, 0, sizeof(used));
	long long low_cost = 0;
	for (int i = 1; i <= m; ++i) {
		if (used[i]) {
			continue;
		}
		map < long long, long long > starts;
		map < long long, long long > finishes;

		for (int j = 1; j <= m; ++j) {
			if (g[i][j]) {
				used[j] = true;
				starts[o[j]] += p[j];
				finishes[-e[j]] += p[j];
			}
		}

		while (!starts.empty()) {
			pair < long long, long long > current = *starts.begin();
			starts.erase(starts.begin());
			while (current.second > 0) {
				pair < long long, long long > ending = *finishes.begin();
				long long d = min(ending.second, current.second);
				current.second -= d;

				low_cost += getCost(-ending.first - current.first) * d;
				low_cost %= P;

				finishes[ending.first] -= d;
				if (d == ending.second) {
					finishes.erase(finishes.begin());
				}
			}
		}
	}

	cerr << trivial << ": " << low_cost << endl;

	long long res = total - low_cost;
	res = ((res % P) + P) % P;*/

	//cout << "Case #" << test << ": " << res << endl;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d\n", &tests);
	for (int i = 1; i <= tests; ++i) {
		solve(i);
		cerr << i << endl;
	}

	return 0;
}