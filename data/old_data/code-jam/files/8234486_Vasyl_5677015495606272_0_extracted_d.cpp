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

Int R[128][4][2][2][2];

void add(Int& a, Int b)
{
	a = (a + b) % MOD;
}

int SolveTest(int test)
{
	int r, c;
	scanf("%d%d", &r, &c);

	CLEAR(R, 0);

	R[0][0][0][0][0] = 1;

	int i, j, k, l, m;
	FOR(i, 0, r)
		FOR(j, 0, 4)
		FOR(k, 0, 2)
		FOR(l, 0, 2)
		FOR(m, 0, 2)
		if (R[i][j][k][l][m] != 0)
		{
			Int v = R[i][j][k][l][m];
			if (j != 2)
				add(R[i + 1][2][k][l][m], v);
			if (j != 3)
				add(R[i + 2][3][k][l][m], v);

			if (j != 2 && c % 4 == 0)
			{
				int kk = k;
				int ll = l;
				Int mm = 1;

				if (k != 0)
					mm = 4;
				else
				{
					if (l != 0)
						mm = 2;

					kk = 1;
				}

				add(R[i + 3][2][kk][ll][m], v * mm);
			}

			if (j != 2 && c % 6 == 0)
			{
				int kk = k;
				int ll = l;
				Int mm = 1;

				if (l != 0)
					mm = 6;
				else
				{
					if (k != 0)
						mm = 2;
					
					ll = 1;
				}

				add(R[i + 2][2][kk][ll][m], v * mm);
			}

			if (j != 2 && c % 3 == 0)
			{
				int kk = k;
				int ll = l;
				int m2 = m;
				Int mm = 1;

				if (m != 0)
					mm = 3;
				else
				{
					m2 = 1;
				}

				add(R[i + 2][2][kk][ll][m2], v * mm);
			}
		}

	Int res = 0;
	FOR(j, 0, 4)
		FOR(k, 0, 2)
		FOR(l, 0, 2)
		FOR(m, 0, 2)
		add(res, R[r][j][k][l][m]);

	printf("Case #%d: %lld\n", test + 1, res);
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
