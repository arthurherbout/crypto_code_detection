#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 6e3;
int N;
int D;

ll V[MAXN];

ll go() {

	assert(N >= 2 * D);

	if(N == 2 * D) {
		V[2 * D] = V[0];
		N++;
	}
	if(V[2 * D] != V[0]) {
		return -1;
	}

	//cerr << D << '\n';
	//for(int i = 0; i < 2 * D; i++) { cerr << V[i] << ' '; } cerr << '\n';

	for(int i = 0; i + 1 < N; i++) {
		V[i] = V[i + 1] - V[i];
	}

	N--;

	for(int i = 0; i + 2 * D < N; i++) {
		if(V[i] != V[i + 2 * D]) {
			return -1;
		}
	}

	ll res = 0;
	for(int d = D; d; d /= 2) {
		//for(int i = 0; i < 2 * d; i++) cerr << V[i] << ' '; cerr << '\n';
		for(int i = 0; i < d; i++) {
			ll diff = V[i] - V[i + d];
			if(diff % 2 != 0) {
				cerr << "hi\n";
				return -1;
			}
			diff /= 2;
			res += abs(diff);
			V[i] -= diff;
			V[i + d] += diff;
		}
	}

	assert(V[0] == 0);

	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	for(int t = 1; t <= T; t++) {
		cin >> N >> D;
		for(int i = 0; i < N; i++) {
			cin >> V[i];
		}
		ll res = go();
		//cerr << res << '\n';
		//cerr << '\n';

		cout << "Case #" << t << ": ";
		if(res == -1) {
			cout << "CHEATERS!" << '\n';
		} else {
			cout << res << '\n';
		}

	}
	return 0;
}
