#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <bitset>
#include <queue>
#include <cmath>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

void solve(int test) {
	cout << "Case #" << test << ":" << endl;
	long long R, N, M, K;
	cin >> R >> N >> M >> K;
	for (int i = 0; i < R; ++i) {
		vector < int > p(K);
		for (int j = 0; j < K; ++j) {
			cin >> p[j];
		}

		bool found = false;
		for (int a = 2; a <= M && !found; ++a) {
			for (int b = a; b <= M && !found; ++b) {
				for (int c = b; c <= M && !found; ++c) {
					set < int > P;
					for (int mask = 0; mask < 8; ++mask) {
						int current = 1;
						if (mask & 1) current *= a;
						if (mask & 2) current *= b;
						if (mask & 4) current *= c;
						P.insert(current);
					}

					bool nice = true;
					for (int j = 0; j < K; ++j) {
						if (P.count(p[j]) == 0) {
							nice = false;
							break;
						}
					}
					if (nice) {
						cout << a << b << c << endl;
						found = true;
					}
				}
			}
		}
	}
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d\n", &tests);
	for (int i = 1; i <= tests; ++i) {
		solve(i);
	}
	return 0;
}