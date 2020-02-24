#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <queue>
#include <stack>
using namespace std;

typedef long long ll;

struct Card {
	int c, s, t, thr;

	friend istream& operator>>(istream& in, Card& c) {
		in >> c.c >> c.s >> c.t;
		return in;
	}

	bool operator<(const Card& o) const {
		if (thr != o.thr) return thr < o.thr;
		if (t != o.t) return t > o.t;
		if (c != o.c) return c > o.c;
		return s > o.s;
	}
} cards[81];
int N, M, f[81][81][80 * 50 + 1];

inline void upd(int& r, int s) {
	if (s > r) r = s;
}

string solve() {
	cin >> N;
	int c0 = 0, t0 = 1, s0 = 0;
	for (int i = 0; i < N; ++i) {
		cin >> cards[i];
		cards[i].thr = 0;
		/*
		if (cards[i].t > 0) {
			c0 += cards[i].c;
			s0 += cards[i].s;
			t0 += cards[i].t - 1;
			--N;
			--i;
		}
		*/
	}
	cin >> M;
	for (int i = N; i < N + M; ++i) {
		cin >> cards[i];
		cards[i].thr = i - N + 1;
	}
	sort(cards, cards + N + M);
	int ans = 0;
	for (int i = 0; i <= N + M; ++i)
		for (int j = 0; j <= N + M; ++j)
			fill(f[i][j], f[i][j] + (N + M) * 50 + 1, -1);
	{
		int c1 = c0, s1 = s0, t1 = t0, n1 = min(N + c0, N + M);
		for (int i = 0; i < n1; ++i)
			if (cards[i].t > 0) {
				s1 += cards[i].s;
				n1 = min(n1 + cards[i].c, N + M);
				t1 += cards[i].t - 1;
			}
		f[0][n1][t1] = s1;
	}
	for (int n = N; n <= N + M; ++n)
		for (int k = 0; k <= n; ++k)
			for (int t = 0; t <= (N + M) * 50; ++t)
				if (f[k][n][t] != -1) {
					upd(ans, f[k][n][t]);
					if (k < n) upd(f[k + 1][n][t], f[k][n][t]);
					if (k < n && t > 0 && cards[k].t == 0) {
						int s1 = f[k][n][t] + cards[k].s;
						int t1 = t - 1;
						int n1 = min(n + cards[k].c, N + M);
						for (int i = n; i < n1; ++i)
							if (cards[i].t > 0) {
								s1 += cards[i].s;
								n1 = min(n1 + cards[k].c, N + M);
								t1 += cards[i].t - 1;
							}
						upd(f[k + 1][n1][t1], s1);
					}
				}
	ostringstream out;
	out << ans;
	return out.str();
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        char* file_name = argv[1];
        int len = strlen(file_name);
        if (strcmp(file_name + len - 3, ".in") == 0)
            file_name[len - 3] = 0;
        else if (strcmp(file_name + len - 1, ".") == 0)
            file_name[len - 1] = 0;
        freopen((string(file_name) + ".in").c_str(), "r", stdin);
        freopen((string(file_name) + ".out").c_str(), "w", stdout);
    }
    int cc = 0, ccc = 0;
    for (cin >> ccc; cc < ccc; ++cc)
            cout << "Case #" << cc + 1 << ": " << solve() << endl;
    return 0;
}
