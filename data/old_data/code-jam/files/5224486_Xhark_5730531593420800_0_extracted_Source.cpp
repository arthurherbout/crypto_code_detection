#include <stdio.h>
#include <algorithm>
#include <vector>
#include <memory.h>
#include <math.h>
#include <map>
#include <string>
#include <set>

using namespace std;
typedef long long LL;

const int MAX_N = 500022;

char dat[MAX_N];
int sum[MAX_N];
int N, F, Fp;

int so, sl, sy;
LL Ab(LL x) {
	return (x < 0) ? (-x) : x;
}
void try_sol(int o, int l) { // o/l
	if (so == -1) {
		so = o; sl = l;
		for (int i = 0; i < N - l + 1; i++) {
			if (sum[i] == o) {
				sy = i;
				break;
			}
		}
		return;
	}

	LL XAF = (LL)l * (LL)sl * (LL)F;
	LL ov = (LL)o * (LL)sl * (LL)1000000;
	LL sov = (LL)so * (LL)l * (LL)1000000;

	LL dif_o = Ab(ov - XAF);
	LL dif_so = Ab(sov - XAF);

	if (dif_o == dif_so) {
		for (int i = 0; i < N - l + 1; i++) {
			if (sum[i] == o) {
				if (sy > i) {
					so = o; sl = l;
					sy = i;
				}
				break;
			}
		}
	}
	else if (dif_o < dif_so) {
		for (int i = 0; i < N - l + 1; i++) {
			if (sum[i] == o) {
				so = o; sl = l;
				sy = i;
				break;
			}
		}
	}
}
int main() {
	freopen("C-small-attempt1.in", "r", stdin);
	freopen("C-small-attempt1.out", "w", stdout);
	int T;
	scanf("%d", &T);
	for (int cs = 1; cs <= T; cs++) {
		so = sl = -1;
		scanf("%d", &N);
		scanf("%d.%d", &Fp, &F);
		F = Fp * 1000000 + F;

		scanf("%s", dat);
		for (int i = 0; i <= N; i++) sum[i] = 0;

		for (int i = 1; i <= N; i++) {
			int mn, mx;
			mn = i; mx = 0;
			for (int j = 0; j < N - i + 1; j++) {
				sum[j] = sum[j + 1] + (dat[j] == '1');
				if (sum[j] < mn) mn = sum[j];
				if (sum[j] > mx) mx = sum[j];
			}
			try_sol(mn, i);
			try_sol(mx, i);
			LL cand1 = ((LL)F * (LL)i) / 1000000ll;
			LL cand2 = cand1 + 1;
			if (mn < cand1 && cand1 < mx) try_sol((int)cand1, i);
			if (mn < cand2 && cand2 < mx) try_sol((int)cand2, i);
		}

		//naive
		for (int i = 0; i < N; i++) {
			int o, l;
			o = l = 0;
			for (int j = i; j < N; j++) {
				if (dat[j] == '1') o++;
				l++;

				LL XAF = (LL)l * (LL)sl * (LL)F;
				LL ov = (LL)o * (LL)sl * (LL)1000000;
				LL sov = (LL)so * (LL)l * (LL)1000000;

				LL dif_o = Ab(ov - XAF);
				LL dif_so = Ab(sov - XAF);

				if (dif_o < dif_so) {
					fprintf(stderr, "OH\n");
				}
			}
		}
		printf("Case #%d: %d\n", cs, sy);
	}
	return 0;
}