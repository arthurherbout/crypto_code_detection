#include <iostream>
#include <vector>
using namespace std;

bool ok(int N, int n, vector<int>& arr) {
	for (int i = 0; i < N; i++) {
		if (arr[i] > n * (i+1)) {
			return false;
		}
	}
	return true;
}

int main() {
	int T; cin >> T;
	for (int No = 1; No <= T; No++) {
		int N, C, M; cin >> N >> C >> M;
		vector<int> P(M), B(M);
		for (int m = 0; m < M; m++) {
			cin >> P[m] >> B[m];
			P[m]--; B[m]--;
		}

		vector<int> arr0(N);
		vector<int> arr(N);
		vector<int> arr2(C);
		for (int i = 0; i < M; i++) {
			arr0[P[i]]++;
			arr2[B[i]]++;
		}
		arr[0] = arr0[0];
		for (int j = 1; j < N; j++) {
			arr[j] = arr0[j] + arr0[j-1];
		}

		int l = 0;
		for (int i = 0; i < C; i++) {
			l = max(l, arr2[i] - 1);
		}
		//cerr << "arr0: ";
		//for (int i = 0; i < N; i++) cerr << arr0[i] << " "; cout << endl;
		
		int r = M;
		//	cerr << N << "," << C << "," << M << endl;
		//	cerr << l << "," << r << endl;
		while (l + 1 < r) {
			int c = (l+r) / 2;
			//cerr << l << "," << r << "," << c << endl;
			if (ok(N, c, arr)) {
				r = c;
			} else {
				l = c;
			}
		}
		int y = r;
		int z = 0;
		for (int i = 0; i < N; i++) {
			z += max(0, arr0[i] - r);
		}
		
		cout << "Case #" << No << ": " << y << " " << z << endl;
	}
}
