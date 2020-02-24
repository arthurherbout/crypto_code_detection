using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <cstring>

#define FORI(p, X) for (__typeof( (X).begin() ) p = (X).begin(); p != (X).end(); ++p)
#define ALL(X) (X).begin(), (X).end()
#define PB push_back
#define MP make_pair

const int INF = 0x3f3f3f3f;

typedef pair <int, int> PII;
typedef vector <int> VI;
typedef long long lint;

const int NMAX = 128;

int DP[NMAX][NMAX], A[NMAX];

int go(int st, int dr) {
	if (DP[st][dr] != -1) return DP[st][dr];
	if (st > dr) return 0;
	if (st == dr) return A[dr + 1] - A[st - 1] - 2;

	int i, rez = INF, ans;
	for (i = st; i <= dr; ++i) {
		ans = go(st, i - 1) + go(i + 1, dr) + A[dr + 1] - A[st - 1] - 2;
		rez = min( rez, ans );
	}		

	return DP[st][dr] = rez;
}

int main(void) {
	FILE *fin = fopen("C-large.in", "rt");
	FILE *fout = fopen("output.out", "wt");
	int ncase, NCASE;
	int i, ans, P, Q;

	fscanf(fin, " %d", &NCASE);

	for (ncase = 1; ncase <= NCASE; ++ncase) {

		fscanf(fin, " %d %d", &P, &Q);

		A[0] = 0, A[Q + 1] = P + 1;
		for (i = 1; i <= Q; ++i)
			fscanf(fin, " %d", A + i);
		sort(A, A + i);

		memset(DP, 0xff, sizeof(DP));
		ans = go(1, Q);

		fprintf(fout, "Case #%d: %d\n", ncase, ans);
	}


	fclose(fin);
	fclose(fout);

	return 0;
}
