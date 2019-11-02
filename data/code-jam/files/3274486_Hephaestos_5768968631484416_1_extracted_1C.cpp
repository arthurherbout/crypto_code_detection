#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <utility>
#include <cmath>

const double PI = 3.141592653589793238463;

typedef unsigned long long ull;

using namespace std;

void SolveA(vector<pair<double, double>>& P, ull K) {
	double ans = 0;
	for (int i = 0; i < P.size(); ++i) {
		double x = P[i].second;
		x += P[i].first;
		int k = 1;
		for (int j = P.size() - 1; j >= 0; --j) {
			if (k == K) {
				break;
			}
			if (j == i) {
				continue;
			}
			if (P[j].second > P[i].second) {
				continue;
			}
			++k;
			x += P[j].first;
		}
		if (k == K) {
			ans = max(ans, x);
		}
	}
	cout.precision(10);
	cout << PI * ans;
}

void main() {
	int T;
	cin >> T;
	for (int i = 0; i < T; ++i) {
		ull K, N;
		cin >> N >> K;
		vector<pair<double, double>> P;
		for (int j = 0; j < N; ++j) {
			double r, h;
			cin >> r >> h;
			P.push_back(make_pair(h * 2 * r, r * r));
		}
		sort(P.begin(), P.end());
		cout << "Case #" << i + 1 << ": ";
		SolveA(P, K);
		cout << endl;
	}
}