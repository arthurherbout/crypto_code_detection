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

int n;
vector < int > s;

multiset < int > S;

vector < int > calc(int n) {
	if (n == 1) {
		S.erase(S.find(0));
		vector < int > res;
		res.push_back(*S.begin());
		S.erase(S.begin());
		return res;
	}

	S.erase(S.find(0));
	int value = *S.begin();

	S.insert(0);

	vector < int > updated;
	for (int i = 0; i < (1 << (n - 1)); ++i) {
		int current = *S.rbegin();
		updated.push_back(current - value);
		S.erase(S.find(current));
		S.erase(S.find(current - value));
	}

	for (int i = 0; i < updated.size(); ++i) {
		S.insert(updated[i]);
	}

	vector < int > res = calc(n - 1);
	res.push_back(value);
	return res;
}

void solve(int test) {
	s.clear();
	int p;
	cin >> p;
	vector < int > a(p), b(p);
	for (int i = 0; i < p; ++i) {
		cin >> a[i];
	}
	int sum = 0;
	for (int i = 0; i < p; ++i) {
		cin >> b[i];
		sum += b[i];
	}
	for (int i = 0; ; ++i) {
		if ((1 << i) == sum) {
			n = i;
			break;
		}
	}
	S.clear();
	for (int i = 0; i < p; ++i) {
		for (int j = 0; j < b[i]; ++j) {
			S.insert(a[i]);
		}
	}
	vector < int > res = calc(n);
	sort(res.begin(), res.end());
	printf("Case #%d:", test);
	for (int i = 0; i < res.size(); ++i) {
		printf(" %d", res[i]);
	}
	printf("\n");
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
		}
		cerr << i << ": " << clock() << endl;
	}

	return 0;
}
