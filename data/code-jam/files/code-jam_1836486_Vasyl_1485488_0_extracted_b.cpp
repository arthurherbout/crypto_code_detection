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

const int DX[] = {-1, 1, 0, 0};
const int DY[] = {0, 0, -1, 1};

int A[128][128];
int B[128][128];
int R[128][128];

int SolveTest(int test)
{
	int n, m, h;
	scanf("%d%d%d", &h, &n, &m);

	int i, j, k;
	FOR(i, 0, n)
		FOR(j, 0, m)
			scanf("%d", &B[i][j]);

	FOR(i, 0, n)
		FOR(j, 0, m)
			scanf("%d", &A[i][j]);

	FOR(i, 0, n)
		FOR(j, 0, m)
			R[i][j] = -INF;
	
	R[0][0] = h;
	priority_queue< pair<int, PII> > Q;
	Q.push(MP(R[0][0], PII(0, 0)));
	while(!Q.empty())
	{
		int value = Q.top().first;
		int x = Q.top().second.first;
		int y = Q.top().second.second;
		Q.pop();

		if(R[x][y] != value)
			continue;

		FOR(i, 0, 4)
		{
			int xx = x + DX[i];
			int yy = y + DY[i];
			if(xx < 0 || xx >= n || yy < 0 || yy >= m || B[xx][yy] - A[xx][yy] < 50 || B[x][y] - A[xx][yy] < 50 || B[xx][yy] - A[x][y] < 50)
				continue;

			int res = min(value, B[xx][yy] - 50);
			if(res != h)
			{
				if(res - A[x][y] < 20)
					res -= 100;
				else
					res -= 10;
			}

			if(R[xx][yy] < res)
			{
				R[xx][yy] = res;
				Q.push(MP(R[xx][yy], PII(xx, yy)));
			}
		}
	}

	int ret = h - R[n - 1][m - 1];
	printf("Case #%d: %d.%d\n", test + 1, ret/10, ret % 10);
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
