#include <bits/stdc++.h>
using namespace std;

int T, N, K;
double P[256];
int bca[65535];

int bc(int n)
{
	int c = 0;
	for (int i=1; i<=n; i<<=1)
		if (n & i) c++;
	return c;
}

int main() {
	for (int i=0; i<65536; i++)
		bca[i] = bc(i);
	cin >> T;
	for (int t=1; t<=T; t++) {
		cin >> N >> K;
		for (int i=0; i<N; i++) cin >> P[i];
		double best = 0;
		for (int Kmask=0; Kmask<(1<<N); Kmask++) if (bca[Kmask] == K) {
			double ans=0;
			for (int mask=0; mask<(1<<N); mask++) if ((Kmask & mask) == mask && bca[mask] == K/2) {
				double p = 1;
				for (int i=0; i<N; i++)
					if (Kmask & (1<<i)) {
						if (mask & (1<<i)) p *= P[i]; 
						else p *= (1-P[i]);
					}
				ans += p;
			}
			best = max(best, ans);
		}

		printf("Case #%d: %.8f\n", t, best);
	}
}
