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

const int DX[] = {-1, 1, 0, 0};
const int DY[] = {0, 0, -1, 1};

char A[16][16];
int X[8];
int Y[8];
int X2[8];
int Y2[8];

int F(int N)
{
	int i;
	queue<int> Q;
	int B[8];
	CLEAR(B, 0);
	B[0] = 1;
	Q.push(0);
	int cnt = N - 1;
	while(cnt != 0 && !Q.empty())
	{
		int a = Q.front();
		Q.pop();
		FOR(i, 0, N)
			if(B[i] == 0 && abs(X[i] - X[a]) + abs(Y[i] - Y[a]) == 1)
			{
				B[i] = 1;
				Q.push(i);
				--cnt;
			}
	}

	return cnt == 0 ? 1 : 0;
}

Int Encode(int X[], int Y[], int N)
{
	int i;
	Int res = 0;
	FOR(i, 0, N)
	{
		res <<= 4;
		res |= X[i];
		res <<= 4;
		res |= Y[i];
	}

	return res;
}

void Decode(Int key, int X[], int Y[], int N)
{
	int i;
	RFOR(i, N, 0)
	{
		Y[i] = int(key & 15);
		key >>= 4;
		X[i] = int(key & 15);
		key >>= 4;
	}
}

int SolveTest(int test)
{
	int N, M;
	scanf("%d%d", &N, &M);
	
	int i, j, k;
	FOR(i, 0, N)
		scanf("%s", A[i]);

	int cnt = 0;
	int cnt2 = 0;
	FOR(i, 0, N)
		FOR(j, 0, M)
		{
			if(A[i][j] == 'o' || A[i][j] == 'w')
			{
				X[cnt] = i;
				Y[cnt] = j;
				++cnt;
			}

			if(A[i][j] == 'x' || A[i][j] == 'w')
			{
				X2[cnt2] = i;
				Y2[cnt2] = j;
				++cnt2;
			}
		}

	if(cnt2 != cnt)
	{
		printf("Case #%d: -1\n", test + 1);
		return 0;
	}

	Int key = Encode(X, Y, cnt);

	set<Int> Set;
	queue<Int> Q;
	Q.push(key);
	Set.insert(key);
	int res = 0;
	while(!Q.empty())
	{
		queue<Int> Q2;
		while(!Q.empty())
		{
			key = Q.front();
			Q.pop();
			Decode(key, X, Y, cnt);

			FOR(i, 0, cnt)
			{
				FOR(j, 0, cnt)
					if(X[i] == X2[j] && Y[i] == Y2[j])
						break;

				if(j == cnt)
					break;
			}

			if(i == cnt)
			{
				printf("Case #%d: %d\n", test + 1, res);
				return 0;
			}

			int st = F(cnt);
			FOR(i, 0, cnt)
				FOR(j, 0, 4)
				{
					k = j ^ 1;
					int x = X[i] + DX[j];
					int y = Y[i] + DY[j];
					int xx = X[i] + DX[k];
					int yy = Y[i] + DY[k];

					if(x < 0 || x >= N || y < 0 || y >= M || A[x][y] == '#')
						continue;
					if(xx < 0 || xx >= N || yy < 0 || yy >= M || A[xx][yy] == '#')
						continue;

					FOR(k, 0, cnt)
						if((X[k] == x && Y[k] == y) || ((X[k] == xx && Y[k] == yy)))
							break;

					if(k < cnt)
						continue;

					xx = X[i];
					yy = Y[i];

					X[i] = x;
					Y[i] = y;

					if(st || F(cnt))
					{
						key = Encode(X, Y, cnt);
						if(Set.find(key) == Set.end())
						{
							Set.insert(key);
							Q2.push(key);
						}
					}

					X[i] = xx;
					Y[i] = yy;
				}
		}

		++res;
		Q = Q2;
	}

	printf("Case #%d: -1\n", test + 1);
	return 0;
}

int main()
{
	freopen("a.in", "r", stdin);
	freopen("a.out", "w", stdout);

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
