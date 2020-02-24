#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

struct attack {
	int d, w, e, s, id;
	attack(int d, int w, int e, int s, int id) : d(d), w(w), e(e), s(s), id(id) {}
	bool operator<(const attack& a) const {
		return d < a.d;
	}
};

int main() {
	int T; cin >> T;
	for (int caseNo = 1; caseNo <= T; caseNo++) {
		int N; cin >> N;
		vector<int> d(N), n(N), w(N), e(N), s(N), delta_d(N), delta_p(N), delta_s(N);
		for (int i = 0; i < N; i++) {
			cin >> d[i] >> n[i] >> w[i] >> e[i] >> s[i] >> delta_d[i] >> delta_p[i] >> delta_s[i];
		}

		vector<attack> allAttacks;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < n[i]; j++) {
				int D = d[i] + delta_d[i] * j;
				int W = w[i] + delta_p[i] * j;
				int E = e[i] + delta_p[i] * j;
				int S = s[i] + delta_s[i] * j;
				allAttacks.push_back(attack(D, W, E, S, i));
			}
		}

		sort(allAttacks.begin(), allAttacks.end());

		map<int, int> height;
		map<int, int> tmpHeight;
		int prevDay = -1;
		int ans = 0;

		for (int i = 0; i < allAttacks.size(); i++) {
			int D = allAttacks[i].d;
			int W = allAttacks[i].w;
			int E = allAttacks[i].e;
			int S = allAttacks[i].s;
			int ID = allAttacks[i].id;
			if (prevDay < D) {
				prevDay = D;
				height = tmpHeight;
			}
			bool success = false;
			for (int j = W; j < E; j++) {
				if (height[j] < S) {
					success = true;
					break;
				}
			}
			if (success) {
				ans++;
				for (int j = W; j < E; j++) {
					tmpHeight[j] = max(tmpHeight[j], S);
				}
				//cerr << "Attack Succeeded! id=" << ID << " [" << W << "," << E << "] s=" << S << endl;
			} else {
				//cerr << "Attack Failed! id=" << ID << " [" << W << "," << E << "] s=" << S << endl;
			}
		}

		cout << "Case #" << caseNo << ": " << ans << endl;
	}
	return 0;
}
