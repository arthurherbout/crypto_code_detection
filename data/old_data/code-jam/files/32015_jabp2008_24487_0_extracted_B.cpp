#include <iostream>
#include <string>

using namespace std;

void backtrack(int &T, string &D, int i,
		int M2, int M3, int M5, int M7,
		int m2, int m3, int m5, int m7) {
	int L = D.length();
	if (i+1 < L) {
		int d = (D[i+1] - '0');
		backtrack(T, D, i+1,
			(M2 + m2) % 2, (M3 + m3) % 3, (M5 + m5) % 5, (M7 + m7) % 7,
			d % 2, d % 3, d % 5, d % 7);
		backtrack(T, D, i+1,
			(M2 - m2) % 2, (M3 - m3) % 3, (M5 - m5) % 5, (M7 - m7) % 7,
			d % 2, d % 3, d % 5, d % 7);
		backtrack(T, D, i+1,
			M2, M3, M5, M7,
			(m2 * 10 + d) % 2, (m3 * 10 + d) % 3, (m5 * 10 + d) % 5, (m7 * 10 + d) % 7);
	} else {
		M2 = (M2 + m2) % 2;
		M3 = (M3 + m3) % 3;
		M5 = (M5 + m5) % 5;
		M7 = (M7 + m7) % 7;
		if ((M2 == 0) || (M3 == 0) || (M5 == 0) || (M7 == 0))
			T++;
	}
}

int main(void) {
	int N; cin >> N;
	for (int x = 1; x <= N; x++) {
		string D; cin >> D;
		int T = 0;
		int d = (D[0] - '0');
		backtrack(T, D, 0, 0, 0, 0, 0, d % 2, d % 3, d % 5, d % 7);
		cout << "Case #" << x << ": " << T << endl;
	}
	return 0;
}
