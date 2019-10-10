#include <iostream>
using namespace std;

long long solve(int n) {
	long long i, j;
	bool used[10] = {false};

	for (i = n; ; i += n) {
		long long x = i;
		while (x > 0) {
			used[x % 10] = true;
			x /= 10;
		}
		for (j = 0; j < 10; j++) if (!used[j]) break;
		if (j == 10) return i;
	}
	return -1;
}

int main() {
	int t, n;
	
	cin >> t;
	for (int id = 1; id <= t; id++) {
		cin >> n;
		cout << "Case #" << id << ": ";
		if (n > 0)
			cout << solve(n) << endl;
		else
			cout << "INSOMNIA" << endl;
	}
	return 0;
}