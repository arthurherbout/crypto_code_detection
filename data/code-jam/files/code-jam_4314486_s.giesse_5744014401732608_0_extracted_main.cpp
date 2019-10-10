#include <iostream>
#include <vector>

using namespace std;

int testcase() {
	int b,m;
	cin >> b >> m;

	vector<int> count(b);
	for (int i = 0; i < b; ++i) {
		count[i] = 1 << (b-i-2);
	}
	count[b-1] = 0;

	vector<vector<int> > graph(b);
	for (int i = 0; i < b; ++i) {
		graph[i].resize(b);
		for (int j = 0; j <= i; ++j) graph[i][j] = 0;
		for (int j = i+1; j < b; ++j) graph[i][j] = 1;
	}

	bool possible = (m <= count[0]);
	while (possible and count[0] > m) {
		int d = count[0] - m;
		int i = 1;
		for (; i < b; ++i) {
		    if (count[i] <= d and graph[0][i] == 1) {
				graph[0][i] = 0;
				count[0] -= count[i];
				break;
			}
		}
	}

	if (possible) {
		cout << "POSSIBLE" << endl;
		for (int i = 0; i < b; ++i) {
			for (int j = 0; j < b; ++j) cout << graph[i][j];
			if (i < b-1) cout << endl;
		}
	}
	else {
		cout << "IMPOSSIBLE";
	}
	
    return 0;
}

int main() {
	int t;
	cin >> t;
	for (int i = 1; i <= t; ++i) {
	    cout << "Case #" << i << ": ";
		int n = testcase();
	    cout << endl;
	}
}
