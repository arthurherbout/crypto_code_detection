#include <bits/stdc++.h>

#define debug(x) cout << #x << " = " << x << endl
#define fori(i, ini, lim) for(int i = int(ini); i < int(lim); i++)
#define ford(i, ini, lim) for(int i = int(ini); i >= int(lim); i--)

using namespace std;

typedef long long ll;
typedef pair<int, int> ii;
typedef tuple<int, int, int, int, int> state;

const int MAX = 35;
vector<ii> edges;
map<state, bool> memo;
map<state, int> parent;
map<state, int> mark;
int F, P;
int pass = 1;

bool roll(int idx, int a, int b, int c, int d) {
	if(idx == (int) edges.size()) {
		return a == 0 && b == 0 && c == 0 && d == 0;
	}
	state s = state(idx, a, b, c, d);
	bool &ans = memo[s];
	if(mark[s] == pass) {
		return ans;
	}
	mark[s] = pass;
	ans = false;
	fori(i, -F * F, F * F + 1) {
		if(i == 0) {
			continue;
		}
		int u = edges[idx].first, v = edges[idx].second;
		bool ret = roll(idx + 1, a + i * (u == 1) + -i * (v == 1), b + i * (u == 2) + -i * (v == 2), c + i * (u == 3) + -i * (v == 3), d + i * (u == 4) + -i * (v == 4));
		if(ret) {
			parent[state(idx, a, b, c, d)] = i;
			ans = true;
			break;
		}
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(false);

	int t;
	cin >> t;
	int kase = 1;
	while(t--) {
		cin >> F >> P;
		edges.clear();
		fori(i, 0, P) {
			int u, v;
			cin >> u >> v;
			edges.emplace_back(u, v);
		}
		bool ans = roll(0, 0, 0, 0, 0);
		cout << "Case #" << kase++ << ": ";
		if(ans) {
			state cur = state(0, 0, 0, 0, 0);
			vector<int> V;
			while(get<0>(cur) < (int) edges.size()) {
				int p = parent[cur];
				V.push_back(p);
				int idx, a, b, c, d;
				tie(idx, a, b, c, d) = cur;
				int u, v;
				tie(u, v) = edges[idx];
				cur = state(idx + 1, a + p * (u == 1) + -p * (v == 1), b + p * (u == 2) + -p * (v == 2), c + p * (u == 3) + -p * (v == 3), d + p * (u == 4) + -p * (v == 4));
			}
			fori(i, 0, V.size()) {
				if(i) {
					cout << " ";
				}
				cout << V[i];
			}
			cout << '\n';
		}
		else {
			cout << "IMPOSSIBLE" << '\n';
		}
		pass++;
	}

	return 0;
}

