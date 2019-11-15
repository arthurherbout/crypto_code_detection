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

int H[128];
int G[128];
int R[128][1 << 11][2];

int SolveTest(int test)
{
	int P, Q, N;
	scanf("%d%d%d", &P, &Q, &N);

	int i, j, k, l, m;
	FOR(i, 0, N)
		scanf("%d%d", &H[i], &G[i]);

	CLEAR(R, -1);
	R[0][1][0] = 0;

	FOR(i, 0, N)
	{
		int limit = 11 * i + 11;
		FOR(j, 0, limit)
			FOR(k, 0, 2)
			if (R[i][j][k] != -1)
			{
				int h = H[i];
				int cnt = max(0, (h + P - 1) / P);
				FOR(l, 0, 1)
				{
					int left = h - l * P;
					int tower = (left + Q - 1) / Q;

					int j2 = j + tower - l;
					if (j2 < 1)
						continue;

					R[i + 1][j2][0] = max(R[i + 1][j2][0], R[i][j][k]);
				}

				cnt = max(0, (h + Q - 1) / Q);
				FOR(l, cnt - 1, cnt)
				{
					int left = h - l * Q;
					int diana = (left + P - 1) / P;

					int j2 = j + l - diana;
					if (j2 < 0)
						continue;

					R[i + 1][j2][1] = max(R[i + 1][j2][1], R[i][j][k] + G[i]);
				}
			}
	}

	int res = 0;
	int limit = 11 * N + 11;
	FOR(i, 0, limit)
	{
		res = max(res, R[N][i][0]);
		res = max(res, R[N][i][1]);
	}
	
	printf("Case #%d: %d\n", test + 1, res);
	return 0;
}

int main()
{
	freopen("b.in", "r", stdin);
	freopen("b.out", "w", stdout);

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
