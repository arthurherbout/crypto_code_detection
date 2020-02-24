#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

int dp[81][200][200];
int c[81];
int s[81];
int t[81];
int N;
int H;

int solve(int i, int turns, int extra) {
	if (i >= N || i >= H+extra || turns == 0) {
		return 0;
	}
	int& ret = dp[i][turns][extra];
	if(ret != -1) {
		return ret;
	}
	// Don't play it.
	ret = solve(i+1, turns, extra);
	if(t[i] != 0) {
		return ret;
	}

	// Play it.
	turns--;
	turns+=t[i];
	int score = s[i];


	int oldextra = extra;
	extra+=c[i];
	for(int j = H+oldextra; j<N; j++) {
		if(j < H+extra && t[j] != 0) {
			turns--;
			turns+=t[j];
			score+=s[j];
			extra+=c[j];
		}
	}




	if(turns>N-i-1) {
		turns = N-i-1;
	}
	if(H+extra > N) {
		extra = N-H;
	}



	score += solve(i+1, turns, extra);
	if(score > ret) {
		ret = score;
	}
	return ret;
}

struct card {
	int c;
	int s;
	bool operator<(const card& o) const {
		return c > o.c;
	}
};
card cards[100];
int main() {
	int TESTS;
	scanf("%d", &TESTS);
	for(int TEST = 1; TEST<=TESTS; TEST++) {
		int score = 0;
		N = 0;
		H = 0;
		int startextra = 0;
		int A;
		scanf("%d", &A);
		int turns = 1;
		for(int i = 0; i<A; i++) {
			int cc, ss, tt;
			scanf("%d%d%d", &cc, &ss, &tt);
			if(tt != 0) {
				score += ss;
				turns += tt-1;
				startextra+=cc;
			} else {
				c[N] = cc;
				s[N] = ss;
				t[N] = tt;
				N++;
				H++;
			}
		}
		int B;
		scanf("%d", &B);
		for(int i = 0; i<B; i++) {
			int cc, ss, tt;
			scanf("%d%d%d", &cc, &ss, &tt);
			if(i < startextra) {
				if(tt != 0) {
					score += ss;
					turns += tt-1;
					startextra+=cc;
				} else {
					c[N] = cc;
					s[N] = ss;
					t[N] = tt;
					N++;
					H++;
				}
			} else {
				c[N] = cc;
				s[N] = ss;
				t[N] = tt;
				N++;
			}
		}
		if(turns > N) {
			turns = N;
		}
		for(int i = 0; i<H; i++) {
			cards[i].c = c[i];
			cards[i].s = s[i];
		}
		sort(cards, cards+H);
		for(int i = 0; i<H; i++) {
			c[i] = cards[i].c;
			s[i] = cards[i].s;
		}
		memset(dp, -1, sizeof(dp));
		printf("Case #%d: %d\n", TEST, score + solve(0, turns, 0));
	}
	return 0;
}
