#include <iostream>
#include <string>
#include <cstring>
#include <set>
#include <map>
#include <vector>
#include <bitset>
#include <queue>
#include <algorithm>
#include <cstdio>
#pragma comment(linker, "/STACK:256000000")

using namespace std;

const int MAXI = 10000;

using Seq = vector<int>;

vector<int> merge(vector<Seq>& a) {
	int sz = 0;
	for (int i = 0; i < a.size(); ++i) {
		sz += a[i].size();
	}

	vector<int> vals(sz);
	for (int i = 0; i < vals.size(); ++i) {
		vals[i] = i;
	}
	random_shuffle(vals.begin(), vals.end());

	vector<int> ans(sz);
	for (int i = 0; i < a.size(); ++i) {
		vector<int> cur;
		for (int j = 0; j < a[i].size(); ++j) {
			cur.push_back(vals.back());
			vals.pop_back();
		}
		sort(cur.begin(), cur.end());
		for (int j = 0; j < a[i].size(); ++j) {
			ans[cur[j]] = a[i][j];
		}
	}
	return ans;
}

vector<Seq> genrand(vector<vector<int>>& g, int v) {
	if (g[v].size() == 0) {
		vector<Seq> ret;
		Seq ans;
		ans.push_back(v);
		ret.push_back(ans);
		return ret;
	}

	vector<vector<Seq> > ans;
	for (int i = 0; i < g[v].size(); ++i) {
		ans.push_back(genrand(g, g[v][i]));
	}

	vector<Seq> ret;
	
	for (int i = 0; i < MAXI; ++i) {
		vector<Seq> tomerge;
		for (int j = 0; j < ans.size(); ++j) {
			tomerge.push_back(ans[j][rand() % ans[j].size()]);
		}
		Seq res = merge(tomerge);
		res.insert(res.begin(), v);
		ret.push_back(res);
	}
	return ret;
}

vector<double> solve(const vector<int>& a, vector<string>& words, const string& s) {
	vector<vector<int> > inv(a.size(), vector<int>(a.size()));

	vector<vector<int> > g(a.size() + 1);
	for (int i = 0; i < a.size(); ++i) {
		if (a[i] != -1) {
			g[a[i]].push_back(i);
		}
	}

	for (int i = 0; i < a.size(); ++i) {
		if (a[i] == -1) {
			g[a.size()].push_back(i);
		}
	}

	vector<Seq> randstrings = genrand(g, a.size());

	vector<double> total(words.size(), randstrings.size());
	vector<double> good(words.size(), 0);

	for (int i = 0; i < randstrings.size(); ++i) {
		Seq cur = randstrings[i];
		cur.erase(cur.begin());
		string res = "";
		for (int j = 0; j < cur.size(); ++j) {
			res += s[cur[j]];
		}

		for (int j = 0; j < words.size(); ++j) {
			if (res.find(words[j]) != string::npos) {
				++good[j];
			}
		}
	}

	vector<double> ans(words.size());
	for (int i = 0; i < ans.size(); ++i) {
		ans[i] = good[i] / total[i];
	}
	return ans;
}

void solve(int tcase) {
	int n;
	cin >> n;
	vector<int> a(n);
	for (int i = 0; i < n; ++i) {
		cin >> a[i];
		--a[i];
	}

	string s;
	cin >> s;

	int m;
	cin >> m;
	vector<string> words(m);
	for (int i = 0; i < m; ++i) {
		cin >> words[i];
	}

	cout << "Case #" << tcase << ":";

	vector<double> res = solve(a, words, s);

	for (int i = 0; i < m; ++i) {
		cout << " " << res[i];
	}
	cout << endl;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int t;
	cin >> t;

	for (int i = 0; i < t; ++i) {
		cerr << i << endl;
		solve(i + 1);
	}

	return 0;
}