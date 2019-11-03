#include <iostream>
#include <vector>

using namespace std;

typedef long long INT;
typedef vector<INT> vi;

#define LIMIT 1000000007

int main(void) {
	int N; cin >> N;
	for (int x = 1; x <= N; x++) {
		INT n, m, X, Y, Z;
		cin >> n >> m >> X >> Y >> Z;
		vi A(m);
		for (INT i = 0; i < m; i++) {
			cin >> A[i];
		}
		vi S;
		for (INT i = 0; i < n; i++) {
			S.push_back(A[i % m]);
			A[i % m] = (X * A[i % m] + Y * (i + 1)) % Z;
		}
		vi T(n, 1);
		for (INT i = 0; i < n; i++) {
			for (INT j = i - 1; j >= 0; j--) {
				if (S[i] > S[j]) {
					T[i] += T[j] % LIMIT;
					T[i] %= LIMIT;
				}
			}
		}
		INT TT = 0;
		for (INT i = 0; i < n; i++) {
			TT += T[i] % LIMIT;
			TT %= LIMIT;
		}
		cout << "Case #" << x << ": " << TT << endl;
	}
	return 0;
}
