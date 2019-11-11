#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <bitset>
#include <cmath>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

void readData() {
}

const int maxN = 110;
int d[maxN][maxN];
string s;

int calc(int l, int r) {
	if (l > r) {
		return 0;
	}
	if (d[l][r] >= 0) {
		return d[l][r];
	}

	int res = 0;
	for (int i = l + 1; i <= r; i += 2) {
		int score = (s[l] == s[i] ? 10 : 5);
		res = max(res, score + calc(l + 1, i - 1) + calc(i + 1, r));
	}
	return d[l][r] = res;
}

void bruteforce(vector < pair < int, char > > items, int len, int score, int &res) {
	/*if (len == s.size()) {
		res = max(res, score);
		return;
	}

	items.push_back(make_pair(s[len] == 'C' ? 10 : 5, 'C'));
	bruteforce(items, len + 1, score, res);
	items.pop_back();
	items.push_back(make_pair(s[len] == 'J' ? 10 : 5, 'J'));
	bruteforce(items, len + 1, score, res);
	items.pop_back();

	if (items.size() > 0) {
		int cur = items.back().first;
		if (s[len] != items.back().second) {
			cur -= 5;
		}
		pair < int, char > b = items.back();
		items.pop_back();
		bruteforce(items, len + 1, cur + score, res);
		items.push_back(b);
	}*/
}

int trivialSolve(string s) {
	int res = 0;
	vector < pair < int, char > > current;
	for (int i = 0; i < s.size(); ++i) {
		if (current.size() > 0 && current.back().second == s[i]) {
			res += current.back().first;
			current.pop_back();
			continue;
		}

		if (current.size() == s.size() - i) {
			res += current.back().first - 5;
			current.pop_back();
			continue;
		}
		current.push_back(make_pair(10, s[i]));
	}
	return res;
}

void solve(int test) {
	cin >> s;

	/*if (trivialSolve(s) != calc(0, s.size() - 1)) {
		cerr << "Incorrect: " << test << endl;
	}*/

	printf("Case #%d: %d\n", test, trivialSolve(s));
	return;
}

int main(int argc, char* argv[]) {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int left_bound, right_bound;

	/*freopen(argv[3], "w", stdout);	
	sscanf(argv[1], "%d", &left_bound);
	sscanf(argv[2], "%d", &right_bound);*/

	int t;
	cin >> t;
	left_bound = 1, right_bound = t;
	for (int i = 1; i <= t; ++i) {
		if (i >= left_bound && i <= right_bound) {
			solve(i);
		} else {
			readData();
		}
		cerr << i << ": " << clock() << endl;
	}

	return 0;
}
