#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <numeric>
using namespace std;

#define FOR(i,a,b) for(int i=(a); i<(int)(b); ++i)
#define ALL(a) (a).begin(),(a).end()
#define PB(a) push_back(a)
#define MP(a,b) make_pair((a),(b))
#define sqr(a) ((a)*(a))
typedef long long i64;
typedef unsigned long long u64;

int nextInt() {
	int x;
	scanf("%d", &x);
	return x;
}

long long nextLong() {
	long long x;
	scanf("%I64d", &x);
	return x;
}

double nextDouble() {
	double x;
	scanf("%lf", &x);
	return x;
}

const int BUFSIZE = 1000000;
char buf[BUFSIZE + 1];
string nextString() {
	scanf("%s", buf);
	return buf;
}

typedef vector<vector<int> > Adj;

const int MOD = 1000003;

int getIndex(int i, int j, int width) {
	return i * width + j;
}

int newId(int height, int width, int i, int j) {
	i = (i % height + height) % height;
	j = (j % width + width) % width;
	return getIndex(i, j, width);
}

Adj createGraph(int height, int width, vector<string>& grid) {
	Adj res(height * width, vector<int>());
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int id = getIndex(i, j, width);
			if (grid[i][j] == '-') {
				res[id].push_back(newId(height, width, i, j - 1));
				res[id].push_back(newId(height, width, i, j + 1));
			} else if (grid[i][j] == '|') {
				res[id].push_back(newId(height, width, i + 1, j));
				res[id].push_back(newId(height, width, i - 1, j));
			} else if (grid[i][j] == '/') {
				res[id].push_back(newId(height, width, i + 1, j - 1));
				res[id].push_back(newId(height, width, i - 1, j + 1));
			} else if (grid[i][j] == '\\') {
				res[id].push_back(newId(height, width, i + 1, j + 1));
				res[id].push_back(newId(height, width, i - 1, j - 1));
			} else {
				throw 0;
			}	
		}
	}
	return res;
}


bool valid(vector<int> &a) {
	sort(a.begin(), a.end());
	for (int i = 1; i < a.size(); ++i) {
		if (a[i] == a[i - 1]) {
			return false;
		}
	}
	return true;
}

int solve(int height, int width, vector<string> &grid) {
	Adj g = createGraph(height, width, grid);
	int res = 0;
	for (int mask = 0; mask < (1 << g.size()); ++mask) {
		vector<int> next(g.size());
		for (int i = 0; i < g.size(); ++i) {
			if (((mask >> i) & 1) != 0) {
				next[i] = g[i][0];
			} else {
				next[i] = g[i][1];
			}
		}
		res += valid(next);
	}
	return res % MOD;
}


int main() {
	freopen("C-small-attempt0.in", "rt", stdin);
	freopen("C-small-attempt0.out", "wt", stdout);
	int T = nextInt();
	for (int cas = 1; cas <= T; ++cas) {
		int height = nextInt();
		int width = nextInt();
		vector<string> grid(height);
		for (int i = 0; i < height; ++i) {
			cin >> grid[i];
		}
		int res = solve(height, width, grid);		
		cerr << cas << endl;
		printf("Case #%d: %d\n", cas, res);
	}
	return 0;
}

