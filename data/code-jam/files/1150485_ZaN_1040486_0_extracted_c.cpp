#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

int n, m;
struct room {
	vector<int> v;
};
vector<room> rooms;
vector<char> vis;
vector<vector<int> > sides;
vector<int> c;
set<int> allc;

bool color(int m, int r) {
	if (r >= rooms.size()) {
		return true;
	}
	vis[r] = true;
	set<int> rem = allc;
	int left = rooms[r].v.size();
	int empty = -1;
	for (int t = 0; t < rooms[r].v.size(); ++t) {
		if (c[rooms[r].v[t]] != -1) {
			--left;
			rem.erase(c[rooms[r].v[t]]);
		} else if (empty == -1) {
			empty = rooms[r].v[t];
		}
	}
	if (rem.size() == 0) {
		if (color(m, r + 1)) {
			return true;
		}
	} else if (rem.size() > left) {
		return false;
	} else {
		for (set<int>::iterator it = rem.begin(); it != rem.end(); ++it) {
			c[empty] = *it;
			if (color(m, r)) {
				return true;
			}
			c[empty] = -1;
		}
	}
	vis[r] = false;
	return false;
}

int main() {
	int t;
	cin >> t;
	for (int test = 1; test <= t; ++test) {
		cin >> n >> m;
		vector<char> out(n, 0);
		vector<char> wout(m, 0);
		vector<int> u(m), v(m);
		vector<vector<int> > w(n);

		sides.clear();
		sides.resize(m);
		c.resize(n);
		for (int i = 0; i < n; ++i) {
			w[i].resize(n, -1);
		}

		for (int j = 0; j < m; ++j) {
			cin >> u[j];
			--u[j];
		}
		for (int j = 0; j < m; ++j) {
			cin >> v[j];
			--v[j];
			w[u[j]][v[j]] = w[v[j]][u[j]] = j;
		}
		
		rooms.clear();
		bool found = true;
		while (found) {
			found = false;
			for (int i = 0; i < n; ++i) {
				if (out[i]) {
					continue;
				}
				
				for (int k = i; k < i + n; ++k) {
					int l = k % n;
					int e = w[i][l];
					if (out[k] || e == -1 || wout[e]) {
						continue;
					}
					
					found = true;
					wout[e] = true;
					rooms.resize(rooms.size() + 1);
					for (int t = i; t <= k; ++t) {
						int s = t % n;
						if (!out[s]) {
							rooms.back().v.push_back(s);
							if (s != i && s != l) {
								out[s] = true;
							}
						}
					}
					break;
				}
			}
		}
		
		rooms.resize(rooms.size() + 1);
		for (int i = 0; i < n; ++i) {
			if (!out[i]) {
				rooms.back().v.push_back(i);
			}
		}
		vis.resize(rooms.size());
		
		for (int j = 0; j < rooms.size(); ++j) {
			for (int t = 1; t < rooms[j].v.size(); ++t) {
				int e = w[rooms[j].v[t - 1]][rooms[j].v[t]];
				if (e != -1) {
					sides[e].push_back(j);
				}
			}
			int e = w[rooms[j].v[0]][rooms[j].v.back()];
			if (e != -1) {
				sides[e].push_back(j);
			}
		}

		int ans = n;
		int minr = 0;
		for (int j = 0; j < rooms.size(); ++j) {
			if (ans > rooms[j].v.size()) {
				ans = rooms[j].v.size();
				minr = j;
			}
		}

		found = false;
		do {
			allc.clear();
			for (int i = 0; i < ans; ++i) {
				allc.insert(i);
			}
			c.assign(n, -1);
			vis.assign(rooms.size(), false);
			found = color(ans, 0);
			if (!found) {
				--ans;
			}
		} while (!found);
		
		cout << "Case #" << test << ": " << ans << endl;
		for (int i = 0; i < n; ++i) {
			cout << (i > 0 ? " " : "") << (c[i] == -1 ? 1 : c[i] + 1);
		}
		cout << endl;
	}
}
