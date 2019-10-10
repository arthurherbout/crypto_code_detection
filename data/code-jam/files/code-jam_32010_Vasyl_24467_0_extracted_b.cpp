#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <memory>
#include <cctype>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
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

#define INF (1 << 30)

int Res[303][303][303];

vector<string> Color;
int GetColor(string s)
{
	int i;
	FOR(i, 0, SIZE(Color))
		if(Color[i] == s)
			break;

	if(i == SIZE(Color))
		Color.PB(s);

	return i;
}

pair<PII, int> A[512];

int SolveTest(int test)
{
	Color.clear();
	int N;
	scanf("%d", &N);
	
	int i, j, k, l, ii;
	FOR(i, 0, N)
	{
		int a, b;
		char buf[32];

		scanf("%s%d%d", buf, &a, &b);
		--a;

		A[i] = MP(PII(b, a), GetColor(buf));
	}

	sort(A, A + N);

	int M = SIZE(Color);
	FOR(i, 0, N)
		FOR(j, 0, M + 1)
			FOR(k, 0, M + 1)
				Res[i][j][k] = INF;

	int res = INF;
	FOR(i, 0, N)
	{
		int a = A[i].first.second;
		int b = A[i].first.first;
		int c = A[i].second;

		if(a == 0)
			Res[i][M][M] = 1;

		FOR(j, 0, M + 1)
			FOR(k, 0, M + 1)
				if(Res[i][j][k] != INF)
				{
					if(b == 10000)
						res = min(res, Res[i][j][k]);

					FOR(l, i + 1, N)
					{
						int aa = A[l].first.second;
						int bb = A[l].first.first;
						int cc = A[l].second;

						if(bb <= b || aa > b)
							continue;

						int temp[] = {c, j, k, cc};
						int t[] = {M, M, M, M};
						sort(temp, temp + 4);

						int cnt = 0;
						FOR(ii, 0, 4)
							if(temp[ii] != cc && temp[ii] != M && (ii == 0 || temp[ii] != temp[ii - 1]))
								t[cnt++] = temp[ii];

						if(cnt > 2)
							continue;

						Res[l][ t[0] ][ t[1] ] = min(Res[l][ t[0] ][ t[1] ], Res[i][j][k] + 1);
					}
				}
	}

	if(res == INF)
		printf("Case #%d: IMPOSSIBLE\n", test + 1);
	else
		printf("Case #%d: %d\n", test + 1, res);

	return 0;
}

int main()
{
	freopen("b.in", "r", stdin);
	freopen("b.out", "w", stdout);

	char buf[1 << 10];
	gets(buf);
	int T, t;
	sscanf(buf, "%d", &T);
	FOR(t, 0, T)
	{
		fprintf(stderr, "Test %d of %d\n", t + 1, T);
		SolveTest(t);
	}

	return 0;
};
