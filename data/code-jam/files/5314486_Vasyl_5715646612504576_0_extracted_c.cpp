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

const int DX[] = { -1, 1, 0, 0 };
const int DY[] = { 0, 0, -1, 1 };

int X[3 << 10];
int Y[3 << 10];
VInt E[6 << 10];
int B[6 << 10];
bool G[6 << 10][6 << 10];
int V[6 << 10];

char A[64][64];
VInt Z[64][64];
int R, C;
int gCnt;

bool f()
{
	int i, j, k;
	FOR(i, 0, gCnt)
	{
		vector<PII> cells[2];
		bool fails[] = { false, false };
		FOR(j, 0, 4)
		{
			int pos = j / 2;
			int r = X[i];
			int c = Y[i];
			int dir = j;
			while (true)
			{
				r += DX[dir];
				c += DY[dir];
				if (r < 0 || R <= r || c < 0 || C <= c || A[r][c] == '#') break;
				if (A[r][c] == '|' || A[r][c] == '-')
				{
					fails[pos] = true;
					break;
				}
				if (A[r][c] == '.')
				{
					cells[pos].push_back(PII(r, c));
				}
				else
				{
					dir ^= 2;
					if (A[r][c] == '/') dir ^= 1;
				}
			}
		}

		FOR(j, 0, 2)
		{
			if (fails[j])
			{
				E[i * 2 + j].push_back(i * 2 + (j ^ 1));
				continue;
			}
			FOR(k, 0, SIZE(cells[j])) Z[cells[j][k].first][cells[j][k].second].push_back(i * 2 + j);
		}
	}

	FOR(i, 0, R)
		FOR(j, 0, C)
		if(A[i][j] == '.')
		{
			int sz = SIZE(Z[i][j]);
			if (sz > 2) throw 0;
			if (sz == 0) return false;
			if(sz == 1)
			{
				int a = Z[i][j][0];
				E[a ^ 1].push_back(a);
			}
			else
			{
				int a = Z[i][j][0];
				int b = Z[i][j][1];
				E[a ^ 1].push_back(b);
				E[b ^ 1].push_back(a);
			}
		}

	CLEAR(G, 0);
	FOR(i, 0, 2*gCnt)
	{
		CLEAR(B, 0);
		B[i] = 1;
		queue<int> q;
		q.push(i);
		G[i][i] = true;
		while(!q.empty())
		{
			int a = q.front();
			q.pop();
			FOR(j, 0, SIZE(E[a]))
			{
				int b = E[a][j];
				if(B[b] == 0)
				{
					B[b] = 1;
					G[i][b] = true;
					q.push(b);
				}
			}
		}
	}

	FOR(i, 0, 2 * gCnt) if (G[i][i ^ 1] && G[i ^ 1][i]) return false;

	CLEAR(V, -1);
	FOR(i, 0, 2*gCnt)
		if(V[i >> 1] == -1 && !G[i][i ^ 1])
		{
			V[i >> 1] = i & 1;
			queue<int> q;
			q.push(i);
			while(!q.empty())
			{
				int a = q.front();
				q.pop();
				FOR(j, 0, SIZE(E[a]))
				{
					int b = E[a][j];
					if(V[b >> 1] == -1)
					{
						V[b >> 1] = b & 1;
						q.push(b);
					}
					if (V[b >> 1] != (b & 1)) throw 0;
				}
			}
		}

	return true;
}

int SolveTest(int test)
{
	scanf("%d%d", &R, &C);

	int i, j;
	FOR(i, 0, R)
		scanf("%s", A[i]);

	gCnt = 0;
	FOR(i, 0, R)
		FOR(j, 0, C)
		if(A[i][j] == '-' || A[i][j] == '|')
		{
			X[gCnt] = i;
			Y[gCnt] = j;
			++gCnt;
		}

	bool res = f();
	FOR(i, 0, R)
		FOR(j, 0, C)
		Z[i][j].clear();

	FOR(i, 0, gCnt * 2)
		E[i].clear();

	printf("Case #%d: %s\n", test + 1, res ? "POSSIBLE" : "IMPOSSIBLE");
	if(res)
	{
		FOR(i, 0, gCnt)
			A[X[i]][Y[i]] = V[i] == 0 ? '|' : '-';
		FOR(i, 0, R)
			A[i][C] = 0;
		FOR(i, 0, R)
			printf("%s\n", A[i]);
	}
	return 0;
}

int main()
{
	freopen("", "r", stdin);
	freopen("", "w", stdout);

	int T, t;
	char buf[1 << 7];
	fgets(buf, 1 << 7, stdin);
	sscanf(buf, "%d", &T);
	FOR(t, 0, T)
	{
		fprintf(stderr, "Solving %d/%d\n", t + 1, T);
		SolveTest(t);
	}

	return 0;
};
