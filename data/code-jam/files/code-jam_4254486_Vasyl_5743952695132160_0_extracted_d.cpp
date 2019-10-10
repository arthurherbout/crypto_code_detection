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

Int E[1 << 20];
Int F[1 << 20];
Int L[1 << 20];
Int R[1 << 20];

int check(int P, int N, Int value)
{
	Int v = value < 0 ? -value : value;
	swap(v, value);

	int i;
	FOR(i, 0, P)
	{
		L[i] = F[i];
		R[i] = 0;
	}

	int pos = 0;
	FOR(i, 0, P)
		if (L[i] != 0)
		{
			while (pos != P && E[i] + value > E[pos])
				++pos;

			if (pos == P || E[i] + value != E[pos] || L[pos] < L[i])
				return 0;

			L[pos] -= L[i];

			if (v > 0)
				R[i] += L[i];
			else
				R[pos] += L[i];
		}

	return 1;
}

int SolveTest(int test)
{
	int P;
	scanf("%d", &P);

	int i, j, k;
	FOR(i, 0, P)
		scanf("%lld", &E[i]);
	FOR(i, 0, P)
		scanf("%lld", &F[i]);

	int cntZero = 64;
	int posZero = 0;
	FOR(i, 0, P)
	{
		if (E[i] == 0)
			posZero = i;

		int cnt = 0;
		int f = F[i];

		while ((f & 1) == 0)
		{
			f >>= 1;
			++cnt;
		}

		cntZero = min(cntZero, cnt);
	}

	FOR(i, 0, P)
		F[i] >>= cntZero;

	Int sum = 0;
	FOR(i, 0, P)
		sum += F[i];

	int N = 0;
	while ((1LL << N) < sum)
		++N;

	vector<Int> v(cntZero, 0LL);
	FOR(i, 0, N)
	{
		FOR(j, 0, P)
			if (E[j] != 0 && F[j] != 0 && check(P, N, E[j]) != 0)
				break;

		if (j == P)
			throw 0;

		v.push_back(E[j]);

		FOR(j, 0, P)
			F[j] = R[j];
	}

	if (F[posZero] != 1)
		throw 0;

	sort(ALL(v));
	printf("Case #%d: ", test + 1);
	FOR(i, 0, SIZE(v))
		printf("%lld%c", v[i], i == SIZE(v) - 1 ? '\n' : ' ');

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
