#include <iostream>
#include <queue>

using namespace std;


const int MAXN = 512;

struct bitset {
	unsigned a[MAXN/32];

	void init() {
		memset (a, 0, sizeof a);
	}

	bool at (int pos) {
		return (a[pos>>5] & (1u << (pos & 31))) != 0;
	}

	bool any (int l, int r) {
		for (; l <= r && (l & 31); ++l)
			if (at (l))
				return true;
		for (; l <= r && ((r+1) & 31); --r)
			if (at (r))
				return true;
		for (int pos = l >> 5; l <= r; l += 32, pos++)
			if (a[pos])
				return true;
		return false;
	}

	void set (int pos) {
		a[pos>>5] |= (1u << (pos & 31));
	}

	void fill (int l, int r) {
		for (; l <= r && (l & 31); ++l)
			set (l);
		for (; l <= r && ((r+1) & 31); --r)
			set (r);
		for (int pos = l >> 5; l <= r; l += 32, pos++)
			a[pos] = -1;
	}
};

bool a[MAXN][MAXN];
bitset row[MAXN];
int drow[MAXN][MAXN], dcol[MAXN][MAXN], d[MAXN][MAXN];
priority_queue < pair<int, pair<int,int> > > q;

int main() {
	freopen ("input.txt", "rt", stdin);
	freopen ("output.txt", "wt", stdout);

	int ts;
	cin >> ts;
	for (int tt=1; tt<=ts; ++tt) {

		int n, m;
		cin >> n >> m;
		for (int i=0; i<n; ++i)
			row[i].init();
		for (int i=0; i<n; ++i)
			for (int j=0; j<m; j+=4) {
				char c;
				scanf (" %c", &c);
				int d = ('0' <= c && c <= '9') ? (c - '0') : (c - 'A' + 10);
				for (int k=0; k<4; ++k) {
					int x = i,  y = j + k,  val = d & (1 << (3-k));
					a[x][y] = val != 0;
				}
			}

		for (int i=n-1; i>=0; --i)
			for (int j=m-1; j>=0; --j) {
				drow[i][j] = 1 + (j+1 < m && a[i][j] != a[i][j+1] ? drow[i][j+1] : 0);
				dcol[i][j] = 1 + (i+1 < n && a[i][j] != a[i+1][j] ? dcol[i+1][j] : 0);
				d[i][j] = min (min (drow[i][j], dcol[i][j]), 1 + (i+1<n && j+1<m && a[i][j] == a[i+1][j+1] ? d[i+1][j+1] : 0));
				q.push (make_pair (d[i][j], make_pair (-i, -j)));
			}

		vector < pair<int,int> > ans;
		int sum = 0;
		while (q.size()) {
			int sz = q.top().first,
				x = -q.top().second.first,
				y = -q.top().second.second;
			q.pop();

			bool bad = false;
			for (int i=x; i<x+sz; ++i)
				if (row[i].any (y, y+sz-1)) {
					if (sz > 1)
						q.push (make_pair (sz-1, make_pair (-x, -y)));
					bad = true;
					break;
				}
			if (bad)
				continue;

			for (int i=x; i<x+sz; ++i)
				row[i].fill (y, y+sz-1);

			if (ans.size() && ans.back().first == sz)
				++ans.back().second;
			else
				ans.push_back (make_pair (sz, 1));
			sum += sz*sz;
		}

		if (sum != n * m)
			throw;

		printf ("Case #%d: %d\n", tt, ans.size());
		for (size_t i=0; i<ans.size(); ++i)
			printf ("%d %d\n", ans[i].first, ans[i].second);
	}


}

