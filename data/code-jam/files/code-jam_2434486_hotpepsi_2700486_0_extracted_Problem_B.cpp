// Google Code Jam 2013 R1B
// Problem B. 

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <stdio.h>

using namespace std;

typedef long long LL;

typedef vector<LL> LLVec;

#define COMBSZ 60
#define SEQSZ 256

static LL C[COMBSZ][COMBSZ];
static int seq[SEQSZ][SEQSZ*2];
static int sum[SEQSZ];

double solve(int N, int X, int Y)
{
	if (X <= -(SEQSZ-2) || X >= (SEQSZ-2) || Y < 0 || Y >= (SEQSZ-2)) {
		return 0.0;
	}
	int s = seq[Y][SEQSZ+X];
	if (s < 0) {
		return 0.0;
	}
	if (N >= sum[s]) {
		return 1.0;
	}

	int R = min(N - sum[s-1], s * 4 + 1);
	if (R <= 0) {
		return 0.0;
	}
	if (Y >= s*2) {
		return 0.0;
	}

	int x = s * 2;
	while (R > x) {
		--x;
		R -= 2;
		--Y;
		if (Y < 0) {
			return 1.0;
		}
	}

	LL a = 0;
	LL b = 0;
	int i;
	for (i = 0; i <= R; ++i) {
		a += C[R][i];
		if (i >= (Y+1)) {
			b += C[R][i];
		}
	}

	return (double)b / (double)a;
}

int main(int argc, char *argv[])
{
	memset(C, 0, sizeof(C));
	memset(seq, -1, sizeof(seq));
	int i, j, k;
	for (i = 0; i < COMBSZ; ++i) {
		C[i][0] = 1;
	}
	for (i = 1; i < COMBSZ; ++i) {
		for (j = 1; j < COMBSZ; ++j) {
			C[i][j] = (C[i-1][j-1] + C[i-1][j]);
		}
	}

	sum[0] = 1;
	for (i = 0; i < 100; ++i) {
		k = 0;
		for (j = i*2; j >= 0; j -= 1, k += 1) {
			seq[k][SEQSZ+j] = i;
			seq[k][SEQSZ-j] = i;
//			printf("x:%d, y:%d, seq:%d\n", j, k, i);
		}
		if (i > 0) {
			sum[i] = sum[i-1] + i*4+1;
		}
	}

	cout.precision(16);
	string s;
	getline(cin, s);
	int T = atoi(s.c_str());
	if (T <= 0) return 0;

	for (int t = 0; t < T; ++t) {
		int N = 0, X = 0, Y = 0;
		getline(cin, s);
		stringstream ss(s);
		ss >> N >> X >> Y;
		if (N <= 0) {
			break;
		}
//		cout << "Case #" << (t+1) << ": " << solve(N, X, Y) << endl;
		printf("Case #%d: %.7f\n", t+1, solve(N, X, Y));
	}

	return 0;
}

