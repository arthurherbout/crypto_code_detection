#include <iostream>
#include <vector>
#include <map>
#include <set>
using namespace std;

int solve(vector<int>& f) {
	int n = f.size();
	int ans = 0;
	set<pair<int,int> > two;
	map<int,int> power;
	for (int i = 0; i < n; i++) {
		vector<int> depth(n);
		int cur = i;
		int sz = 1;
		while (depth[cur] == 0) {
			depth[cur] = sz;
			sz++;
			cur = f[cur];
		}
		ans = max(ans, sz - depth[cur]);
		if (sz - depth[cur] == 2) {
			//cerr << cur << endl;
			two.insert(make_pair(min(cur, f[cur]), max(cur, f[cur])));
			power[cur] = max(power[cur], sz - 2);
		}
	}
	int twoans = 0;
	for (set<pair<int,int> >::iterator it = two.begin(); it != two.end(); ++it) {
		twoans += power[it->first] + power[it->second];
	}

	return max(ans, twoans);
}

int main() {
	int T; cin >> T;
	for (int No = 1; No <= T; No++) {
		int N; cin >> N;
		vector<int> F(N);
		for (int i = 0; i < N; i++) {
			cin >> F[i];
			F[i]--;
		}
		int ans = solve(F);
		cout << "Case #" << No << ": " << ans << endl;
	}
	return 0;
}
