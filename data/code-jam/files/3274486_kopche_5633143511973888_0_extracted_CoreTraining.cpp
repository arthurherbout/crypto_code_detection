#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>

using namespace std;

int main() {
    freopen("CoreTraining.in", "rt", stdin);
	freopen("CoreTraining.out", "wt", stdout);
	ios_base::sync_with_stdio(false);

    int numTests;
	cin >> numTests;

    for (int t = 0; t < numTests; ++t) {
		int N, K;
		cin >> N >> K;
		double U;
		cin >> U;
		
		
		vector <double> probability(N);
		for (int i = 0; i < N; ++i) {
			cin >> probability[i];
		}
		

		double left = 0.0;
		double right = 1.0;
		for (int i = 0; i < 100; ++i) {
			double mid = (left + right) / 2;
			double needed = 0.0;
			for (int j = 0; j < N; ++j) {
				if (probability[j] < mid) {
					needed += (mid - probability[j]);	
				}
			}
			if (needed <= U) left = mid;
			else right = mid;
		}
		
		double answer = 1.0;
		for (int i = 0; i < N; ++i) {
			answer *= ((probability[i] < left) ? left : probability[i]);
		}
		
		cout.precision(9);
		cout << "Case #" << t + 1 << ": " << fixed << answer << endl;
    }
    
    return 0;
}
