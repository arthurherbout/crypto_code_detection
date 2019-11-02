#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <map>
#include <cstring>
#include <set>
#include <deque>
#include <cmath>
#include <stack>

using namespace std;

int s[2222];
int h[2222];

int n; 
void solve(int b, int e, int step = 0) {
	if (b >= e) return;
	if (s[b] > e) throw 3;

	vector<int> v;
	for (int cur = b; ; cur = s[cur]) {
		v.push_back(cur);
		if (cur == e) break;
	}

	int last = 1000000000;
	if (e != n) {
		last = h[e];
	}

	for (int i=v.size() -1 ; i>=0; i--) {
		int dist = e - v[i];
		h[v[i]] = last - dist * step;
	}

	for (int i=0; i<v.size()-1; i++) {
		solve(v[i]+1, v[i+1], step +1 );
	}
}

int main() {
	//freopen("input.txt", "r", stdin);
	freopen("outputC.txt", "w", stdout);

	int t; cin >> t;
	
	for (int e=1; e<=t; e++) {
		cin >> n;
		for (int i=1; i<=n-1; i++)
			cin >> s[i];

		memset(h, -1, sizeof h);
		cout << "Case #" << e << ": ";
		bool can = true;
		try {
			solve(1, n);
		} catch (...) {
			can = false;
		}

		for (int i=1; i<=n; i++) {
			if (h[i] < 0) {
				can = false;
			}
		}

		if (!can) {
			cout << "Impossible";
		} else {
			for (int i=1; i<=n; i++) {
				cout << h[i] << ' ';
			}
		}

		cout << endl;
	}

    return 0;
}