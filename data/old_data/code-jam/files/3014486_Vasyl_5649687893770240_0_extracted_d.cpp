//#pragma comment(linker, "/STACK:134217728")

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <cstring>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
using namespace std;

typedef long long Int;
typedef pair<int,int> PII;
typedef vector<int> VInt;

#define FOR(i, a, b) for(i = (a); i < (b); ++i)
#define RFOR(i, a, b) for(i = (a) - 1; i >= (b); --i)
#define CLEAR(a, b) memset(a, b, sizeof(a))
#define SIZE(a) int((a).size())
#define ALL(a) (a).begin(),(a).end()
#define PB push_back
#define MP make_pair

#define MOD 1000000007

string S[1 << 10];
char buf[1 << 10];
Int C[1 << 10][1 << 10];
Int R[1 << 10][1 << 10];
Int Sum[1 << 10][1 << 10];
Int Res[1 << 10][1 << 10];
int gN;

int f(int level, int from, int to)
{
	Int& res = R[level][from];
	Int& sum = Sum[level][from];
	if (to - from == 1)
	{
		res = SIZE(S[from]) - level;
		sum = 1;
		return 1;
	}

	int i, j, k;
	int cnt = min(gN, to - from);
	res = cnt;
	sum = 1;

	int all = 0;
	int Max = 0;
	int prev = from;
	
	int c[32];
	Int s[32];
	int am = 0;

	FOR(i, from + 1, to + 1)
	if (i == to || S[i][level] != S[prev][level])
	{
		c[am] = f(level + 1, prev, i);
		s[am] = Sum[level + 1][prev];
		res += R[level + 1][prev];
		sum = sum * s[am] % MOD;

		all += c[am];
		Max = max(Max, c[am]);
		++am;

		prev = i;
	}

	if (Max == cnt)
	{
		FOR(i, 0, am)
		{
			sum *= C[cnt][c[i]] % MOD;
			sum %= MOD;
		}

		return cnt;
	}

	if (all == cnt)
	{
		FOR(i, 0, am)
		{
			sum *= C[all][c[i]] % MOD;
			sum %= MOD;
			
			all -= c[i];
		}

		return cnt;
	}

	FOR(i, 0, am + 1)
		FOR(j, 0, cnt + 1)
		Res[i][j] = 0;

	Res[0][0] = 1;
	FOR(i, 0, am)
		FOR(j, 0, cnt + 1)
		if (Res[i][j] != 0)
			FOR(k, j, cnt + 1)
			{
				int a = k - j;
				int b = c[j] - a;
				if (b < 0 || j < b)
					continue;

				Res[i + 1][k] += (Res[i][j] * (C[j][b] * C[cnt - j][a] % MOD));
				Res[i + 1][k] %= MOD;
			}

	sum *= Res[am][cnt];
	sum %= MOD;
	return cnt;
}

int SolveTest(int test)
{
	int M, N;
	scanf("%d%d", &M, &N);
	gN = N;

	int i, j;
	CLEAR(C, 0);
	FOR(i, 0, 1 << 10)
		C[i][i] = C[i][0] = 1;
	FOR(i, 0, 1 << 10)
		FOR(j, 1, i)
		C[i][j] = (C[i - 1][j] + C[i - 1][j - 1]) % MOD;

	FOR(i, 0, M)
	{
		scanf("%s", buf);
		S[i] = buf;
		S[i] = S[i] + ".";
	}

	f(0, 0, M);
	Int res = R[0][0];
	Int sum = Sum[0][0];

	printf("Case #%d: %d %d\n", test + 1, int(res % MOD), int(sum % MOD));

	return 0;
}

int main()
{
	freopen("d.in", "r", stdin);
	freopen("d.out", "w", stdout);

	int T, t;
	char buf[1 << 7];
	gets(buf);
	sscanf(buf, "%d", &T);
	FOR(t, 0, T)
	{
		fprintf(stderr, "Solving %d/%d\n", t + 1, T);
		SolveTest(t);
	}

	return 0;
};
