#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <set>
#include <map>
using namespace std;
#define MOD
#define ADD(X,Y) ((X) = ((X) + (Y)%MOD) % MOD)
typedef long long i64; typedef vector<int> ivec; typedef vector<string> svec;

int T;
int N, D;
int B[5050];
int tr[5050];
int F, dMax;

bool solve(int p, int d, int ph)
{
	if (p == F) {
		for (int i = 0; i < 2 * D; ++i) if (tr[i] != 0) return false;
		return true;
	}

	for (; d <= dMax; ++d, ph = 0) {
		for (; ph < (2 << d); ++ph) {
			//printf("%d %d %d\n", p, d, ph);
			int pi = ph;
			for (int i = 0; i < 2 * D; ++i) {
				if (pi >= (1 << d) && tr[i] == 0) goto nex;
				pi = (pi + 1) % (2 << d);
			}

			 pi = ph;
			for (int i = 0; i < 2 * D; ++i) {
				if (pi >= (1 << d)) --tr[i];
				pi = (pi + 1) % (2 << d);
			}

			if (solve(p + 1, d, ph)) return true;

			 pi = ph;
			for (int i = 0; i < 2 * D; ++i) {
				if (pi >= (1 << d)) ++tr[i];
				pi = (pi + 1) % (2 << d);
			}
		nex:
			continue;
		}
	}

	return false;
}

int main()
{
	scanf("%d", &T);
	for (int t = 0; t++ < T;){
		scanf("%d%d", &N, &D);
		fprintf(stderr, "%d\n", t);
		for (int i = 0; i < N; ++i) scanf("%d", B + i);
		int mx = 0, mn = 10000;
		for (int i = 0; i < N; ++i) {
			mx = max(mx, B[i]);
			mn = min(mn, B[i]);
		}
		for (int i = 0; i < N; ++i) {
			tr[i] = mx - B[i];
		}

		// mx - mn farmers?
		for (int i = 2 * D; i < N; ++i) if (B[i] != B[i - 2 * D]) goto invalid;

		F = 0;
		for (int i = 0; i < 2 * D; ++i) F += tr[i];
		F /= D;

		for (int i = 0;; ++i) {
			if ((1 << i) == D) {
				dMax = i;
				break;
			}
		}

		for (int t = 0; t < 8; ++t) {
			if (solve(0, 0, 0)) goto good;
			fprintf(stderr, "%d\n", F);
			F += 2;
			for (int i = 0; i < N; ++i) ++tr[i];
		}
		goto invalid;
	good:
		
		printf("Case #%d: %d\n", t, F);
		continue;
	invalid:
		printf("Case #%d: CHEATERS!\n", t);
	}

	return 0;
}
