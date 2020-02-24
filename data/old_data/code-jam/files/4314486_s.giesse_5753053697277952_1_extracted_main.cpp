#include <iostream>
#include <vector>
#include <string>

using namespace std;

int maxparty(vector<int>& p) {
	int max = p[0];
	int best = 0;
	for (int i = 0; i < p.size(); ++i) {
		if (p[i] > max) {
			max = p[i];
			best = i;
		}
	}
	return best;
}

bool absoluteMajority(vector<int>& p, int left) {
    int m = maxparty(p);
	if (p[m] > left/2) return true;
	return false;
}

int testcase() {
	int n;
	cin >> n;
	vector<int> p(n);
	for (int i = 0; i < n; ++i) cin >> p[i];

	int left = 0;
	for (int i = 0; i < n; ++i) left += p[i];

	while (left > 0) {
		int i = maxparty(p);
		p[i]--;
		left--;
		cout << string(1,'A' + i);
		if (absoluteMajority(p, left)) {
			i = maxparty(p);
			p[i]--;
			left--;
			cout << string(1,'A' + i);
		}
		cout << " ";
	}
	
    return 0;
}

int main() {
	int t;
	cin >> t;
	for (int i = 1; i <= t; ++i) {
	    cout << "Case #" << i << ": ";
	    testcase();
		cout << endl;
	}
}
