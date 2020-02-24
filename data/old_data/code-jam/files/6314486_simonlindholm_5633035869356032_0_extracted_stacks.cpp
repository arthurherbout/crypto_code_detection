#include <bits/stdc++.h>
using namespace std;

#define rep(i, from, to) for (int i = from; i < int(to); ++i)
#define trav(a, x) for (auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vector<vector<pii>> sts2;
vector<vector<pii>> sts;

map<vector<vector<pii>>, int> mem;

bool rec(vector<vector<pii>> sts) {
	int& out = mem[sts];
	if (out != 0) return (out == 2);

	bool fail = false;
	rep(i,0,sz(sts)) {
		if (sts[i].size() > 1) fail = true;
	}
	if (!fail) {
		out = 2;
		return true;
	}

	int empty = -1;
	rep(i,0,sz(sts)) {
		if (sts[i].size() == 0) empty = i;
	}

	if (empty != -1) {
		rep(i,0,sz(sts)) {
			if (sts[i].size() > 1) {
				pii x = sts[i].back();
				sts[i].pop_back();
				sts[empty].push_back(x);
				if (rec(sts)) {
					out = 2;
					return true;
				}
				sts[empty].pop_back();
				sts[i].push_back(x);
			}
		}
	}

	rep(i,0,sz(sts)) {
		rep(j,0,sz(sts)) if (i != j) {
			if (!sts[i].empty() && !sts[j].empty() && sts[i].back().second == sts[j].back().second) {
				if (sts[i].back().first < sts[j].back().first) {
					pii x = sts[i].back();
					sts[i].pop_back();
					if (rec(sts)) {
						out = 2;
						return true;
					}
					sts[i].push_back(x);
				}
			}
		}
	}

	out = 1;
	return false;
}

void solve() {
	int N, C;
	cin >> N >> C;

	sts.clear();
	rep(i,0,N) {
		int ind;
		cin >> ind;
		sts.push_back(sts2[ind]);
	}

	if (rec(sts)) {
		cout << "POSSIBLE" << endl;
	} else {
		cout << "IMPOSSIBLE" << endl;
	}
}

int main() {
	cin.sync_with_stdio(false);
	cin.exceptions(cin.failbit | cin.eofbit | cin.badbit);
	cin.tie(0);
	int P;
	cin >> P;
	rep(i,0,P) {
		int C;
		cin >> C;
		vector<pii> st(C);
		rep(j,0,C)
			cin >> st[j].first >> st[j].second; // first = value
		reverse(all(st));
		sts2.push_back(st);
	}

	int T;
	cin >> T;
	rep(i,0,T) {
		cout << "Case #" << i+1 << ": ";
		solve();
	}
}
