#include <bits/stdc++.h>
using namespace std;

int T, N;
int BFF[42], perm[42];

bool ok(int sz) {
	for (int i=0; i<sz; i++)
		if (BFF[perm[i]] != perm[(i+1)%sz] && BFF[perm[i]] != perm[(i+sz-1)%sz])
			return false;
	return true;
}

int main() {
	cin >> T;
	for (int t=1; t<=T; t++) {
		cin >> N;
		for (int i=0; i<N; i++) {
			cin >> BFF[i];
			BFF[i]--;
			perm[i] = i;
		}

		int ans = 1;
		do {
			for (int sz=2; sz<=N; sz++)
				if (ok(sz)) ans = max(ans, sz);
		} while (ans < N && next_permutation(perm, perm+N));

		cout << "Case #" << t << ": " << ans << endl;
	}
}
