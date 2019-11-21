#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <limits>
#include <deque>
#include <cstdint>

using namespace std;


void main() {
	int T;
	cin >> T;
	for (int i = 0; i < T; ++i) {
		int R, C;
		cin >> R >> C;

		int r = R % 3 ? 1 : 2;
		if (R == 2 && C == 3)
			r = 2;
		cout << "Case #" << i + 1 << ": " << r << endl;
	}
}