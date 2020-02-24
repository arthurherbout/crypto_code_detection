//	GCJ 2010 Round 3 (D)

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <utility>
#include <numeric>
#include <algorithm>
#include <bitset>
#include <complex>

using namespace std;

typedef unsigned uint;
typedef long long Int;
typedef vector<int> vint;
typedef vector<string> vstr;
typedef pair<int,int> pint;
#define mp make_pair

template<class T> void pv(T a, T b) { for (T i = a; i != b; ++i) cout << *i << " "; cout << endl; }
template<class T> void pvp(T a, T b) { for (T i = a; i != b; ++i) cout << "(" << i->first << ", " << i->second << ") "; cout << endl; }
void chmin(int &t, int f) { if (t > f) t = f; }
void chmax(int &t, int f) { if (t < f) t = f; }
void chmin(Int &t, Int f) { if (t > f) t = f; }
void chmax(Int &t, Int f) { if (t < f) t = f; }
void chmin(double &t, double f) { if (t > f) t = f; }
void chmax(double &t, double f) { if (t < f) t = f; }
int in_c() { int c; for (; (c = getchar()) <= ' '; ) { if (!~c) throw ~0; } return c; }
int in() { int x = 0, c; for (; (uint)((c = getchar()) - '0') >= 10; ) { if (c == '-') return -in(); if (!~c) throw ~0; } do { x = (x << 3) + (x << 1) + (c - '0'); } while ((uint)((c = getchar()) - '0') < 10); return x; }
Int In() { Int x = 0, c; for (; (uint)((c = getchar()) - '0') >= 10; ) { if (c == '-') return -In(); if (!~c) throw ~0; } do { x = (x << 3) + (x << 1) + (c - '0'); } while ((uint)((c = getchar()) - '0') < 10); return x; }

const Int MO = 1000000007;


Int an[5010][5010];
Int bn[5010][5010];

void prpr() {
	int i, j;
	for (i = 0; i <= 5005; ++i) {
		an[i][0] = 1;
		for (j = 1; j <= i; ++j) {
			an[i][j] = (an[i][j - 1] * (i - j + 1)) % MO;
		}
	}
	for (i = 0; i <= 5005; ++i) {
		bn[i][0] = bn[i][i] = 1;
		for (j = 1; j < i; ++j) {
			bn[i][j] = (bn[i - 1][j - 1] + bn[i - 1][j]) % MO;
		}
	}
} 

Int N;
int B;
int L, A[110];

int BB, BBB, LB;
Int dp[80][5010];

void dping() {
	BB = (B - 1) * B / 2;
	BBB = BB / B;
	LB = L * (B - 1);
	
	int i, j, k;
	memset(dp, 0, sizeof(dp));
	dp[0][0] = 1;
	for (k = 1; k < B; ++k) {
		for (i = B; i--; ) for (j = 0; j <= BB; ++j) {
			dp[i + 1][j + k] += dp[i][j];
			dp[i + 1][j + k] %= MO;
		}
	}
//for(i=0;i<B;++i)pv(dp[i],dp[i]+BB+1);
}

//	pos, carry, allzero
Int DP[110][110][5010];

int main() {
	int i, j, k;
	int jj, x, y;
	
	prpr();
	
	for (int TC = in(), tc = 0; ++tc <= TC; ) {
		N = In();
		B = in();
		for (i = 0; N; ++i) {
			A[i] = N % B;
			N /= B;
		}
		L = i;
		reverse(A, A + L);
//pv(A,A+L);
		
		dping();
		
		memset(DP, 0, sizeof(DP));
		DP[0][0][LB] = 1;
		for (i = 0; i < L; ++i) for (j = 0; j <= BBB; ++j) for (k = 0; k <= LB; ++k) if (DP[i][j][k]) {
//printf("%d %d %d: %lld\n",i,j,k,DP[i][j][k]);
			for (jj = 0; jj <= BBB; ++jj) {
				int tmp = A[i] + j * B - jj;
				if (!(0 <= tmp && tmp <= BB)) continue;
				for (x = LB - B; x <= LB; ++x) if (dp[LB - x][tmp]) {
					for (y = max(x - 1, 0); y <= k && y <= x; ++y) {
						Int c = bn[LB - x][k - y] * an[LB - k][LB - x - (k - y)] % MO;
						c *= dp[LB - x][tmp];
						c %= MO;
						DP[i + 1][jj][y] += DP[i][j][k] * c;
						DP[i + 1][jj][y] %= MO;
					}
				}
			}
		}
		
		Int ans = 0;
		for (k = 0; k <= LB; ++k) if (DP[L][0][k]) {
//cout<<LB-k<<" "<<DP[L][0][k]<<endl;
			ans += DP[L][0][k];
			ans %= MO;
		}
		printf("Case #%d: %lld\n", tc, ans);
		
	}
	
	return 0;
}

