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
LL sum[MAX_N];
LL N, F, Fp;

const LL mult = 1000000ll;

LL Ab(LL x) {
	return (x < 0) ? (-x) : x;
}

bool cmp(LL Aup, LL Adown, LL Bup, LL Bdown) {// <=
	return Aup * Bdown <= Bup * Adown;
}

LL getOne(LL i, LL length) {
	if (i == 0) return sum[length - 1];
	return sum[i + length - 1] - sum[i - 1];
}

bool pos(LL one, LL length) {
	for (int i = 0; i < N - length + 1; i++) {
		if (one == getOne(i, length))
			return true;
	}
	return false;
}

LL so, sl;
void try_sol(LL o, LL l) { // o/l
	if (so == -1) {
		so = o; sl = l;
		return;
	}

	LL XAF = (LL)l * (LL)sl * (LL)F;
	LL ov = (LL)o * (LL)sl * (LL)mult;
	LL sov = (LL)so * (LL)l * (LL)mult;

	LL dif_o = Ab(ov - XAF);
	LL dif_so = Ab(sov - XAF);

	if (dif_o < dif_so) {
		so = o; sl = l;
	}
}

void findSol(LL Aup, LL Adown, LL Bup, LL Bdown) {
	LL Cup = Aup + Bup;
	LL Cdown = Adown + Bdown;

	if (pos(Cup, Cdown)) {
		if (cmp(F, mult, Cup, Cdown)) { // <=
			for (LL step = 2; ; step = step * 2) {
				LL Xup = Aup * step + Bup;
				LL Xdown = Adown * step + Bdown;
				if (Xdown > N) break;
				if (cmp(F, mult, Xup, Xdown)) {
					if (pos(Xup, Xdown)) {
						Cup = Xup;
						Cdown = Xdown;
					}
				}
				else {
					break;
				}
			}
			findSol(Aup, Adown, Cup, Cdown);
		}
		else {
			for (LL step = 2; ; step = step * 2) {
				LL Xup = Aup + Bup * step;
				LL Xdown = Adown + Bdown * step;
				if (Xdown > N) break;
				if (!cmp(F, mult, Xup, Xdown)) {
					if (pos(Xup, Xdown)) {
						Cup = Xup;
						Cdown = Xdown;
					}
				}
				else {
					break;
				}
			}
			findSol(Cup, Cdown, Bup, Bdown);
		}
	}
	else {
		try_sol(Aup, Adown);
		try_sol(Bup, Bdown);
	}
}

LL getSol() {
	// so vs sl

	LL sol = N;
	for (LL i = 0; i < sl; i++) {
		map<LL, LL> pos;
		LL nu = 0;
		pos[0] = i;
		for (LL j = i; j+sl <= N; j += sl) {
			nu += getOne(j, sl) - so;
			if (pos.find(nu) != pos.end()) {
				if (sol > pos[nu]) sol = pos[nu];
			}
			pos[nu] = j + sl;
		}
	}
	return sol;
}
int main() {
	/*
	freopen("input.txt", "a", stdout);
	printf("500000 0.000004\n");
	for (int i = 0; i < 500000; i++) {
		printf("0");
	}
	printf("\n");
	printf("500000 0.000004\n");
	for (int i = 0; i < 500000; i++) {
		if (i == 250000) printf("1");
		else printf("0");
	}
	printf("\n");
	return 0;
	*/
//	freopen("input.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);
	freopen("C-large.in", "r", stdin);
	freopen("C-large.out", "w", stdout);
	int T;
	scanf("%d", &T);
	for (int cs = 1; cs <= T; cs++) {
		so = sl = -1;
		scanf("%lld", &N);
		scanf("%lld.%lld", &Fp, &F);
		F = Fp * mult + F;

		scanf("%s", dat);
		bool one, zero;
		one = zero = false;
		for (int i = 0; i < N; i++) {
			sum[i] = (dat[i] == '1');
			if (i > 0) sum[i] += sum[i - 1];
			if (dat[i] == '0') zero = true;
			if (dat[i] == '1') one = true;
		}
		LL sol;
		if (one && zero) {
			findSol(0, 1, 1, 1);
			
			sol = getSol();
		}
		else {
			sol = 0;
		}

		printf("Case #%d: %lld\n", cs, sol);
	}
	return 0;
}