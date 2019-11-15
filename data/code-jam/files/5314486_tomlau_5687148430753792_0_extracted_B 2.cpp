#include <bits/stdc++.h>
using namespace std;

int T, N, C, M;

int main() {
	cin >> T;
	for (int t=1; t<=T; t++) {
		cin >> N >> C >> M;
		vector <map<int, int>> CP(C);
		vector <int> cnt(C, 0);
		for (int i = 0; i < M; i++) {
			int Pi, Bi;
			cin >> Pi >> Bi;

			CP[Bi-1][Pi]++;
			cnt[Bi-1]++;
		}

		int rides = 0, proms = 0;

		for (int x = 0; x < 2; x++) {
			while (CP[x][1] > 0) {
				rides++;
				CP[x][1]--;
				cnt[x]--;
				bool found = false;
				for (auto &b : CP[1-x])
					if (b.first > 1 && b.second > 0 && CP[x][b.first] > 0) {
						found = true;
						b.second--;
						cnt[1-x]--;
						break;
					}

				if (!found)
					for (auto &b : CP[1-x])
						if (b.first > 1 && b.second > 0) {
							b.second--;
							cnt[1-x]--;
							break;
						}
			}
		}

		int x = (cnt[0] > cnt[1]) ? 1 : 0;
		for (auto &xb : CP[x]) {
			for (int n = 0; n < xb.second; n++) {
				bool found = false;
				for (auto &ob : CP[1 - x])
					if (ob.second > 0 && ob.first != xb.first) {
						ob.second--;
						cnt[1-x]--;
						found = true;
						break;
					}
				if (!found) {
					for (auto &ob : CP[1 - x])
						if (ob.second > 0) {
							ob.second--;
							cnt[1-x]--;
							found = true;
							proms++;
							break;
						}
				}
			}
			cnt[x] -= xb.second;
			rides += xb.second;
			xb.second = 0;
		}

		rides += cnt[0] + cnt[1];

		cout << "Case #" << t << ": " << rides << " " << proms << endl;

		if (cnt[0] > 0 && cnt[1] > 0)
			cout << "ERR!!";
	}
}
