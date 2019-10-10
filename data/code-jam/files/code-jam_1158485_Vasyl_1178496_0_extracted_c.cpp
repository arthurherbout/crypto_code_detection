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

#define MOD 1000003

char A[128][128];
PII E[128][128][2];
PII E2[128][128][2];
PII P[128][128];
vector<PII> Edge[128][128];
int Cnt[128][128];
int D[128][128];
int B[128][128][2];

PII Parent(int x, int y)
{
	if(P[x][y] == PII(x, y))
		return P[x][y];

	return P[x][y] = Parent(P[x][y].first, P[x][y].second);
}

int SolveTest(int test)
{
	int R, C;
	scanf("%d%d", &R, &C);

	int i, j, k, l;
	FOR(i, 0, R)
		scanf("%s", A[i]);

	CLEAR(D, 0);
	CLEAR(Cnt, 0);
	CLEAR(B, 0);
	FOR(i, 0, R)
		FOR(j, 0, C)
			P[i][j] = PII(i, j);

	FOR(i, 0, R)
		FOR(j, 0, C)
		{
			int dx = 0;
			int dy = 0;
			if(A[i][j] == '|')
				dx = 1;
			else if(A[i][j] == '-')
				dy = 1;
			else if(A[i][j] == '\\')
			{
				dx = 1;
				dy = 1;
			}
			else if(A[i][j] == '/')
			{
				dx = 1;
				dy = -1;
			}

			int x = (i + R + dx) % R;
			int y = (j + C + dy) % C;
			E[i][j][0] = PII(x, y);
			x = (i + R - dx) % R;
			y = (j + C - dy) % C;
			E[i][j][1] = PII(x, y);

			FOR(k, 0, 2)
				Edge[ E[i][j][k].first ][ E[i][j][k].second ].PB(PII(i, j));
		}

	FOR(i, 0, R)
		FOR(j, 0, C)
			D[i][j] = Cnt[i][j] = SIZE(Edge[i][j]);

	queue<PII> Q;
	FOR(i, 0, R)
		FOR(j, 0, C)
			if(D[i][j] < 2)
				Q.push(PII(i, j));

	int res = 1;
	while(!Q.empty())
	{
		int x = Q.front().first;
		int y = Q.front().second;
		Q.pop();

		int xx = -1;
		int yy = -1;
		FOR(i, 0, Cnt[x][y])
		{
			xx = Edge[x][y][i].first;
			yy = Edge[x][y][i].second;

			if(B[xx][yy][0] == 0)
				break;
		}

		if(i == Cnt[x][y])
		{
			res = 0;
			break;
		}

		FOR(i, 0, 2)
		{
			int xxx = E[xx][yy][i].first;
			int yyy = E[xx][yy][i].second;

			--D[xxx][yyy];
			if(D[xxx][yyy] == 1)
				Q.push(PII(xxx, yyy));
		}

		B[xx][yy][0] = 1;
		B[x][y][1] = 1;
		P[xx][yy] = PII(x, y);
	}

	FOR(i, 0, R)
		FOR(j, 0, C)
			if(D[i][j] == 2)
			{
				int cnt = 0;
				FOR(k, 0, Cnt[i][j])
				{
					int x = Edge[i][j][k].first;
					int y = Edge[i][j][k].second;

					if(B[x][y][0] == 0)
					{
						E2[i][j][cnt] = PII(x, y);
						++cnt;
					}
				}
			}

	if(res != 0)
		FOR(i, 0, R)
			FOR(j, 0, C)
				FOR(k, 0, 2)
					if(B[i][j][k] == 0)
					{
						queue< pair<PII, int> > Q;
						res += res;
						res %= MOD;
						Q.push(MP(PII(i, j), k));
						B[i][j][k] = 1;
						while(!Q.empty())
						{
							int x = Q.front().first.first;
							int y = Q.front().first.second;
							int z = Q.front().second;
							Q.pop();

							FOR(l, 0, 2)
							{
								int xx = E[x][y][l].first;
								int yy = E[x][y][l].second;
								int zz = z ^ 1;
								if(z)
								{
									xx = E2[x][y][l].first;
									yy = E2[x][y][l].second;
								}

								if(B[xx][yy][zz] == 0)
								{
									B[xx][yy][zz] = 1;
									Q.push(MP(PII(xx, yy), zz));
								}
							}
						}
					}

	FOR(i, 0, R)
		FOR(j, 0, C)
			Edge[i][j].clear();

	printf("Case #%d: %d\n", test + 1, res);
	return 0;
}

int main()
{
	freopen("c.in", "r", stdin);
	freopen("c.out", "w", stdout);

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
