#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <cstdio>
using namespace std;

struct st {
	int n;
	double t;
	st(int n, double t) : n(n), t(t) {}

	bool operator<(const st &x) const {
		if (t != x.t) return t > x.t;
		return n < x.n;
	}
};

int main() {
	int T; cin >> T;
	for (int No = 1; No <= T; No++) {
		int N, Q; cin >> N >> Q;
		vector<int> E(N), S(N);
		for (int i = 0; i < N; i++) {
			cin >> E[i] >> S[i];
		}
		vector<vector<double> > D(N, vector<double>(N));
		for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) cin >> D[i][j];
		for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) if (D[i][j] == -1) D[i][j] = 1e99;
		for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) for (int k = 0; k < N; k++) D[j][k] = min(D[j][k], D[j][i] + D[i][k]);
		//for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) cerr << D[i][j] << " ";
		
		vector<double> ans;
		while (Q--) {
			int U, V; cin >> U >> V; U--; V--;
			priority_queue<st> q;
			set<int> visited;
			q.push(st(U, 0));
			//cerr << "AA" << U << "," << V << endl;
			while (!q.empty()) {
				st s = q.top(); q.pop();
				if (visited.count(s.n)) {
					continue;
				}
				//cerr  << s.n << "," << s.t << endl;
				visited.insert(s.n);
				if (s.n == V) {
					ans.push_back(s.t);
					break;
				}
				for (int i = 0; i < N; i++) {
					if (E[s.n] >= D[s.n][i]) {
						//cerr << s.t << "," << D[s.n][i] << "::" << S[s.n] << endl;
						double newt = s.t + 1.0*D[s.n][i]/S[s.n];
						q.push(st(i, newt));
					}
				}
			}
		}
		cout << "Case #" << No << ": ";
		for (int i = 0; i < ans.size(); i++) {
			if (i != 0) cout << " ";
			printf("%.8f", ans[i]);
		}
		cout << endl;
	}
	return 0;
}
