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

#define FOR(i, a, b) for(i = a; i < b; i++)
#define RFOR(i, a, b) for(i = a - 1; i >= b; i--)
#define CLEAR(a, b) memset(a, b, sizeof(a))
#define SIZE(a) int((a).size()) 
#define ALL(a) (a).begin(),(a).end() 

#define PB push_back
#define MP make_pair

#define INF (1 << 30)

char A[64][64];
int Left[64][64];
int Right[64][64];
int Down[64][64];
int Res[64][64][64];
int N, M, L;

int F(int pos, int from, int to)
{
	if(pos == N - 1)
		return 0;

	int& res = Res[pos][from][to];
	if(res != -1)
		return res;

	res = INF;

	int i, j;
	FOR(i, from, to)
		FOR(j, i + 1, to)
			if(A[pos + 1][j] == '#')
			{
				int value = j - i;
				if(A[pos + 1][i] != '#')
					--value;

				int pos2 = Down[pos + 1][j - 1];
				if(pos2 != pos + 1)
				{
					if(pos2 - pos <= L)
						res = min(res, value + F(pos2, Left[pos2][j - 1], Right[pos2][j - 1]));

					continue;
				}

				int f = i;
				int t = j;
				if(pos + 1 == N - 1 || A[pos + 2][f] == '#')
					f = Left[pos + 1][f];

				res = min(res, value + F(pos + 1, f, t));
			}

	FOR(i, from, to)
		FOR(j, from, i)
			if(A[pos + 1][j] == '#')
			{
				int value = i - j;
				if(A[pos + 1][i] != '#')
					--value;

				int pos2 = Down[pos + 1][j + 1];
				if(pos2 != pos + 1)
				{
					if(pos2 - pos <= L)
						res = min(res, value + F(pos2, Left[pos2][j + 1], Right[pos2][j + 1]));

					continue;
				}

				int f = j + 1;
				int t = i + 1;
				if(pos + 1 == N - 1 || A[pos + 2][t - 1] == '#')
					t = Right[pos + 1][t - 1];

				res = min(res, value + F(pos + 1, f, t));
			}

	return res;
}

int SolveTest(int test)
{
	scanf("%d%d%d", &N, &M, &L);

	int i, j;
	FOR(i, 0, N)
		scanf("%s", A[i]);

	FOR(i, 0, N)
		FOR(j, 0, M)
		{
			int pos = i + 1;
			while(pos != N && A[pos][j] != '#')
				++pos;
			--pos;
			Down[i][j] = pos;
			
			int left = j - 1;
			while(left != -1 && A[pos][left] != '#' && (pos == N - 1 || A[pos + 1][left + 1] == '#'))
				--left;
			Left[i][j] = left + 1;

			int right = j + 1;
			while(right != M && A[pos][right] != '#' && (pos == N - 1 || A[pos + 1][right - 1] == '#'))
				++right;
			Right[i][j] = right;
		}

	CLEAR(Res, -1);
	int res = F(Down[0][0], Left[0][0], Right[0][0]);
	if(res == INF)
		printf("Case #%d: No\n", test + 1);
	else
		printf("Case #%d: Yes %d\n", test + 1, res);

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
		SolveTest(t);

	return 0;
};
