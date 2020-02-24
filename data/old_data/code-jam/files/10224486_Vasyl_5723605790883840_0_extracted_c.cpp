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

int P[256];
int A[128][128];
int M[128][128];
int X[256];
int Y[256];
int Z[256];
int S[256];

int D[128][128][4];

int build(int r, int c, int s, set<int>& Set)
{
	int n = 2 * (r + c);
	CLEAR(D, -1);
	int z = Z[s];
	int x = X[s] + DX[z];
	int y = Y[s] + DY[z];
	D[x + 1][y + 1][z] = 0;
	queue<pair<PII, int>> Q;
	Q.push(MP(PII(x, y), z));
	while(!Q.empty())
	{
		x = Q.front().first.first;
		y = Q.front().first.second;
		z = Q.front().second;
		Q.pop();

		if (x < 0 || r <= x || y < 0 || c <= y) continue;

		int from = A[x][y];
		int to = A[x][y];
		if(from == -1)
		{
			from = 0;
			to = 1;
		}

		int i;
		FOR(i, from, to + 1)
		{
			int zz = z ^ 2 ^ i;
			int xx = x + DX[zz];
			int yy = y + DY[zz];

			if(D[xx + 1][yy + 1][zz] == -1)
			{
				D[xx + 1][yy + 1][zz] = i;
				Q.push(MP(PII(xx, yy), zz));
			}
		}
	}

	x = X[P[s]];
	y = Y[P[s]];
	z = Z[P[s]] ^ 1;
	if (D[x + 1][y + 1][z] == -1) return 0;
	while(x != X[s] && y != Y[s])
	{
		int from = D[x + 1][y + 1][z];
		x -= DX[z];
		y -= DY[z];
		if(A[x][y] == -1)
		{
			A[x][y] = from;
			M[x][y] = s;
		}
		if (A[x][y] != from) return 0;
		z = z ^ 2 ^ from;
	}

	return 1;
}

int SolveTest(int test)
{
	int R, C;
	scanf("%d%d", &R, &C);

	int i, j;
	int n = 2 * (R + C);
	for (i = 0; i < n; i += 2)
	{
		int a, b;
		scanf("%d%d", &a, &b);
		--a;
		--b;
		P[a] = b;
		P[b] = a;
	}

	CLEAR(A, -1);

	FOR(i, 0, n)
	{
		int t = i;
		if(t < C)
		{
			X[i] = -1;
			Y[i] = t;
			Z[i] = 1;
		}
		else if(t < C + R)
		{
			X[i] = t - C;
			Y[i] = C;
			Z[i] = 2;
		}
		else if(t < C + R + C)
		{
			X[i] = R;
			Y[i] = C + R + C - 1 - t;
			Z[i] = 0;
		}
		else
		{
			X[i] = n - 1 - t;
			Y[i] = -1;
			Z[i] = 3;
		}
	}

	CLEAR(S, -1);
	int found = 1;
	while(found)
	{
		found = 0;
		FOR(i, 0, n)
			if(S[i] == -1)
			{
				set<int> Set;
				for (j = (i + 1) % n; j != P[i]; j = (j + 1) % n)
				{
					if (S[j] == -1) break;
					Set.insert(j);
				}

				if(j == P[i])
				{
					if (build(R, C, i, Set) != 0) 
					{
						found = 1;
						S[i] = S[P[i]] = 0;
					}
					break;
				}
			}
	}

	FOR(i, 0, n)
		if (S[i] == -1)
			break;

	printf("Case #%d:\n", test + 1);
	if (i != n)
		printf("IMPOSSIBLE\n");
	else
		FOR(i, 0, R)
		{
			FOR(j, 0, C)
				printf("%c", A[i][j] == 1 ? '/' : '\\');
		
			printf("\n");
		}

	return 0;
}

int main()
{
	freopen("c.in", "r", stdin);
	freopen("c.out", "w", stdout);

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
