#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <queue>
#include <bitset>
#include <cmath>

using namespace std;

const int maxN = 210;

char a[maxN][maxN];
int b[maxN][maxN];

string dirs = "v^><";

int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

bool is_in(int x, int y, int r, int c) {
	return x >= 0 && x < r && y >= 0 && y < c;
}

int fd(char c) {
	for (int i = 0; i < 4; ++i) {
		if (dirs[i] == c) {
			return i;
		}
	}
}

long long hashword(const string& word) {
	long long ret = 0;
	for (int i = 0; i < word.size(); ++i) {
		ret *= 1000003;
		ret += (word[i] - 'a' + 253);
	}
	return ret;
}

map<string, int> res;

int mapword(string& word) {
	if (res.count(word)) {
		return res[word];
	}
	return res[word] = res.size();
}


vector <int> parseline(char* line) {
	int n = strlen(line);
	vector<int> ret;
	string cur = "";
	line[n] = ' ';
	for (int i = 0; i <= n; ++i) {
		if (line[i] == ' ') {
			if (cur != "") {
				ret.push_back(mapword(cur));
				cur = "";
			}
		} else {
			cur += line[i];
		}
	}
	return ret;
}

char sl[50000];


vector <int> g[maxN];
int canbe[maxN];
int both[maxN];

int d[10][10];

void solve(int tcase) {
	printf("Case #%d: ", tcase);
	int n, m;
	cin >> n >> m;
	cout << d[n][m] << endl;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("GCJ2015R2.txt", "w", stdout);

	d[2][3] = 2;
	d[2][4] = 1;
	d[2][5] = 1;
	d[2][6] = 3;
	d[3][3] = 2;
	d[3][4] = 3;
	d[3][5] = 2;
	d[3][6] = 2;
	d[4][3] = 3;
	d[4][4] = 1;
	d[4][5] = 1;
	d[4][6] = 5;
	d[5][3] = 3;
	d[5][4] = 3;
	d[5][5] = 1;
	d[5][6] = 5;
	d[6][3] = 6;
	d[6][4] = 4;
	d[6][5] = 2;
	d[6][6] = 19;
	int t;
	scanf("%d\n", &t);

	for (int i = 1; i <= t; ++i) {
		cerr << i << endl;
		solve(i);
	}


	return 0;
}

