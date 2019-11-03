#include <bits/stdc++.h>
using namespace std;

#define rep(i, from, to) for (int i = from; i < int(to); ++i)
#define trav(a, x) for (auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

typedef ll Res;

struct Solver {
	int N;
	vector<array<int, 6>> v;

	int matched = 0;
	int len = 0;
	vi lmatch, rmatch;
	vector<vi> g;
	vector<bool> seen;

	void read() {
		g.resize(1000001);
		cin >> N;
		v.resize(N);
		rep(i,0,N) {
			rep(j,0,6) {
				cin >> v[i][j];
				g[v[i][j]].push_back(i);
			}
		}
		lmatch.assign(1000001, -1);
		rmatch.assign(N, -1);
	}

	bool find(int j, const vector<vi>& g) {
		if (rmatch[j] == -1) return 1;
		seen[j] = 1;
		int old = rmatch[j];
		trav(e, g[old])
			if (!seen[e] && find(e, g)) {
				lmatch[old] = e;
				rmatch[e] = old;
				return 1;
			}
		return 0;
	}

	/*
	int dfs_matching(const vector<vi>& g, int n, int m) {
		rmatch.assign(m, -1);
		rep(i,0,n) {
			seen.assign(m, 0);
			trav(j,g[i])
				if (find(j, g)) {
					lmatch[i] = j;
					rmatch[j] = i;
					break;
				}
		}
		return m - (int)count(all(rmatch), -1);
	}
	*/

	void extend(int i) {
		seen.assign(N, 0);
		trav(j,g[i])
			if (find(j, g)) {
				lmatch[i] = j;
				rmatch[j] = i;
				++matched;
				break;
			}
	}

	void reduce(int x) {
		if (lmatch[x] == -1) return;
		int r = lmatch[x];
		rmatch[r] = -1;
		--matched;

		seen.assign(N, 0);
		rep(i,x+1,x+len) {
			trav(j,g[i])
				if (lmatch[i] == -1 && find(j, g)) {
					lmatch[i] = j;
					rmatch[j] = i;
					++matched;
					return;
				}
		}
	}

	Res solve() {
		int i = 0, j = 0;
		int res = 1;
		while (i < 1000001) {
			while (j < 1000001) {
				int old = j++;
				extend(old);
				len = j - i;
				if (matched == len)
					res = max(res, j - i);
				else break;
			}
			while (i < j) {
				int old = i++;
				reduce(old);
				len = j - i;
				if (matched == len) break;
			}
		}
		return res;
	}
};

void readSolved(vector<string>& s, const char* fname) {
	ifstream fin(fname);
	string line;
	while (getline(fin, line)) {
		if (line.empty()) continue;
		assert(line.substr(0, 6) == "Case #");
		istringstream iss(line.substr(6));
		int cs;
		char sep;
		string val;
		iss >> cs >> sep >> val;
		s[cs-1] = val;
		{ cerr << "Case #" << cs << ": " << val << endl; }
	}
}

void printsol(ostream& cout, Res x) {
	cout << x << endl;
}

int main() {
	cin.sync_with_stdio(false);
	cin.exceptions(cin.failbit | cin.eofbit | cin.badbit);
	cin.tie(0);
	int T;
	cin >> T;
	vector<Solver> vec(T);
	vector<Res> sol(T);
	vector<string> oldsol(T);
	// readSolved(oldsol, "partial");
	rep(i,0,T) vec[i].read();
#pragma omp parallel for
	rep(i,0,T) {
		if (oldsol[i].empty()) {
			sol[i] = vec[i].solve();
#pragma omp critical
			{ cerr << "Case #" << i+1 << ": "; printsol(cerr, sol[i]); }
		}
	}

	rep(i,0,T) {
		cout << "Case #" << i+1 << ": ";
		if (oldsol[i].empty()) {
			printsol(cout, sol[i]);
		} else {
			cout << oldsol[i] << endl;
		}
	}
}
