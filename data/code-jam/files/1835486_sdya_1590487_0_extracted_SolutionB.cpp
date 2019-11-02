#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <bitset>
#include <cstring>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const int maxS = 100;

int S, m;
int board[maxS][maxS];
int isIn[maxS][maxS];
int used[maxS][maxS];

string ans[8] = {
	"none",
	"ring",
	"bridge",
	"bridge-ring",
	"fork",
	"fork-ring",
	"bridge-fork",
	"bridge-fork-ring"
};

void getIsIn() {
	memset(isIn, 0, sizeof(isIn));
	for (int i = 1; i <= S; ++i) {
		isIn[i][i] = 1;
		for (int j = 1; j < S; ++j) {
			isIn[i + j][i] = 1;
			isIn[i][i + j] = 1;
		}
	}

	for (int i = 1; i < S; ++i) {
		isIn[S + i][S + i] = 1;
		for (int j = 1; j < S - i; ++j) {
			isIn[S + i + j][S + i] = isIn[S + i][S + i + j] = 1;
		}
	}
}

int dx[] = {1, -1, 1, -1, 0, 0};
int dy[] = {1, -1, 0, 0, -1, 1};

int is_in(int x, int y) {
	if (x >= 0 && y >= 0 && isIn[x][y]) return 1;
	return 0;
}

void dfs(int x, int y, int value) {
	if (used[x][y]) return;
	used[x][y] = value;

	for (int i = 0; i < 6; ++i) {
		int cx = x + dx[i];
		int cy = y + dy[i];
		if (is_in(cx, cy)) {
			if (board[cx][cy] == board[x][y]) {
				dfs(cx, cy, value);
			}
		}
	}
}

vector < int > neg;

void updateComponents() {
	int t = 1;
	neg.clear();
	memset(used, 0, sizeof(used));
	for (int i = 0; i < maxS; ++i) {
		for (int j = 0; j < maxS; ++j) {
			if (isIn[i][j] && !used[i][j]) {
				if (board[i][j]) {
					dfs(i, j, t);
					++t;
				} else {
					neg.push_back(-t);
					dfs(i, j, -t);
					++t;
				}
			}
		}
	}
}

int checkRing() {
	set < int > backColors;
	for (int i = 1; i <= S; ++i) {
		backColors.insert(used[1][i]);
		backColors.insert(used[i][1]);
	}

	for (int i = 1; i <= S; ++i) {
		backColors.insert(used[S - 1 + i][i]);
		backColors.insert(used[i][S - 1 + i]);
	}

	for (int i = 1; i <= S; ++i) {
		backColors.insert(used[2 * S - 1][2 * S - i]);
		backColors.insert(used[2 * S - i][2 * S - 1]);
	}

	for (int i = 0; i < neg.size(); ++i) {
		if (!backColors.count(neg[i])) {
			return 1;
		}
	}
	return 0;
}

int checkBridge() {
	set < int > cur;
	vector < pair < int, int > > corners;
	corners.push_back(make_pair(1, 1));
	corners.push_back(make_pair(2 * S - 1, 2 * S - 1));
	corners.push_back(make_pair(1, S));
	corners.push_back(make_pair(S, 1));
	corners.push_back(make_pair(2 * S - 1, S));
	corners.push_back(make_pair(S, 2 * S - 1));

	for (int i = 0; i < 6; ++i) {
		int x = corners[i].first;
		int y = corners[i].second;
		if (used[x][y] > 0) {
			if (cur.count(used[x][y])) return 1;
			cur.insert(used[x][y]);
		}
	}
	return 0;
}



int checkFork() {
	set < int > edges[6];
	for (int i = 2; i < S; ++i) {
		edges[0].insert(used[1][i]);
		edges[1].insert(used[i][1]);
	}

	for (int i = 2; i < S; ++i) {
		edges[2].insert(used[S - 1 + i][i]);
		edges[3].insert(used[i][S - 1 + i]);
	}

	for (int i = 2; i < S; ++i) {
		edges[4].insert(used[2 * S - 1][2 * S - i]);
		edges[5].insert(used[2 * S - i][2 * S - 1]);
	}

	map < int, int > mp;
	for (int i = 0; i < 6; ++i) {
		for (set < int > :: iterator it = edges[i].begin(); it != edges[i].end(); ++it) {
			++mp[*it];
		}
	}

	for (map < int, int > :: iterator it = mp.begin(); it != mp.end(); ++it) {
		int x = it->first;
		int y = it->second;
		if (x > 0 && y >= 3) return 1;
	}
	return 0;
}

void solve(int test) {
	memset(board, 0, sizeof(board));
	printf("Case #%d: ", test);
	scanf("%d%d", &S, &m);

	getIsIn();

	int ok = 0;
	for (int i = 1; i <= m; ++i) {
		int x, y;
		scanf("%d%d", &x, &y);
		board[x][y] = 1;
		updateComponents();
		int res = 0;
		res += checkRing();
		res += checkBridge() * 2;
		res += checkFork() * 4;

		if (res != 0 && !ok) {
			cout << ans[res];
			printf(" in move %d\n", i);
			ok = 1;
		}
	}
	if (!ok) printf("none\n");
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d", &tests);
	for (int i = 0; i < tests; ++i) {
		solve(i + 1);
		cerr << i << endl;
	}
	return 0;
}