#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

int n;
int p[26];

void input() {
	cin >> n;
	for (int i = 0; i < n; i++) cin >> p[i];
}

vector<string> solve() {
	typedef pair<int, int> P;	//(val, type)
	P state[26];
	int i; int cnt = 0;
	vector<string> ret;
	
	for (i = 0; i < n; i++) { state[i] = P(p[i], i); cnt += p[i]; }
	while (cnt > 0) {
		string ss;
		sort(state, state + n, greater<P>());
		ss += 'A' + state[0].second; state[0].first--; cnt--;
		if (state[1].first > 0) {
			state[1].first--; cnt--;
			
			int maxi = 0;
			for (i = 0; i < n; i++) { maxi = max(state[i].first, maxi); }
			if (maxi * 2 > cnt) { state[1].first++; cnt++; }
			else { ss += 'A' + state[1].second; }
		}
		ret.push_back(ss);
	}
	return ret;
}

int main() {
	int t;
	cin >> t;
	for (int id = 1; id <= t; id++) {
		input();
		cout << "Case #" << id << ": ";
		vector<string> ans = solve();
		
		if (ans.size() > 0) {
			cout << ans[0];
			for (int i = 1; i < ans.size(); i++)
				cout << " " << ans[i];
			cout << endl;
		}
	}
	return 0;
}