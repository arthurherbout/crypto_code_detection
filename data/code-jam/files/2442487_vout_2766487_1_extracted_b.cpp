#include <iostream>

using namespace std;

long long highest(long long x, int n) {
	long long ret = 0;
	int i = 0;
	while (x >= (1LL << i)) {
		x -= (1LL << i);
		n--;
		i++;
	}
	return (1LL << n) - 1;
}

long long lowest(long long x, int n) {
	long long ret = 0;
	int i = 0;
	
	while (x >= (1LL << i)) {
		x -= (1LL << i);
		n--;
		i++;
		ret = (ret << 1) + 1;
	}
	return ret << n;
}

int main() {
	int T;
	int n;
	long long p;
	cin >> T;
	for (int re = 1; re <= T; ++re) {
		cin >> n >> p;
		p--;

		long long l = 0, h = (1LL << n) - 1;
//		cerr << highest(h, n) << endl;
		while (h > l) {
			long long m = (l + h) / 2;
			if (highest(m, n) <= p) {
				h = m;
			} else {
				l = m + 1;
			}
		}
		long long b = (1LL << n) - 1 - h;

		l = 0, h = (1LL << n) - 1;
//		cerr << lowest(h, n) << endl;

		while (h > l) {
			long long m = (l + h + 1) / 2;
			if (lowest(m, n) <= p) {
				l = m;
			} else {
				h = m - 1;
			}
		}
		long long a = l;

		cout << "Case #" << re << ": " << a << ' ' << b << endl;
	}

	return 0;
}
