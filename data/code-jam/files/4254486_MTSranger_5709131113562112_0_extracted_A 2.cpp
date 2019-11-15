#include <algorithm>
#include <iostream>
#include <queue>
#include <cstring>

using namespace std;
typedef long long ll;

int main() {
	int ncase; cin >> ncase;
	for (int csnum = 1; csnum <= ncase; csnum++) {
		int n; cin >> n;
		int d; cin >> d;

		ll s0, as, cs, rs; cin >> s0 >> as >> cs >> rs;
		ll m0, am, cm, rm; cin >> m0 >> am >> cm >> rm;

		int sal[n];
		int man[n];

		sal[0] = (int)s0;
		man[0] = -1;

		ll currS = s0;
		ll currM = m0;
		for (int i = 1; i < n; i++) {
			currS = (currS * as + cs) % rs;
			currM = (currM * am + cm) % rm;
			sal[i] = (int)currS;
			man[i] = (int)currM % i;
		}

		vector<int> adj[n];
		for (int i = 1; i < n; i++) {
			adj[man[i]].push_back(i);
		}

		/*
		cerr << "Input for case #" << csnum << endl;
		for (int i = 0; i < n; i++) {
			cerr << sal[i] << ' ' << man[i] << endl;
		}

		cerr << "Hierarchy: " << endl;
		for (int i = 0; i < n; i++) {
			cerr << i <<":";
			for (int j = 0; j < adj[i].size(); j++) {
				cerr << ' ' << adj[i][j];
			}
			cerr << endl;
		}
		*/

		bool dead[n];
		int ans = 1;
		for (int start = 0; start < rs; start++) {
			int end = start + d;
			if (start > 0 && end >= rs) break;
			if (s0 < start || s0 > end) continue;
			memset(dead, 0, sizeof dead);
			for (int i = 1; i < n; i++) {
				if (sal[i] < start || sal[i] > end) {
					queue<int> lol;
					lol.push(i);
					while (!lol.empty()) {
						int curr = lol.front(); lol.pop();
						if (!dead[curr]) {
							dead[curr] = true;
							for (int k = 0; k < adj[curr].size(); k++) {
								lol.push(adj[curr][k]);
							}
						}
					}
				}
			}
			//cerr << "[" << start << " " << end << "]: ";
			int count = 0;
			for (int i = 0; i < n; i++) {
				if (!dead[i]) {
					count++;
					//cerr << i << " ";
				}
			}
			//cerr << endl;
			ans = max(ans, count);
		}
		cout << "Case #" << csnum << ": " << ans << endl;
	}
}
