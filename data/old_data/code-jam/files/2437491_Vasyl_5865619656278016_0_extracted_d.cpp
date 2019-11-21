#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker, "/STACK:134217728")

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

int A[1 << 17][4];
int R[1 << 17][4];
map<PII, int> Map;

int N, D, K;

int find(int a, int b)
{
	if(a == N - 1)
		return N;

	int& res = R[a][b];
	if(res != -1)
		return res;

	int i;
	FOR(i, 0, D)
		if(A[a + 1][i] == A[a][b])
			return res = find(a + 1, i);

	return res = a + 1;
}

int find2(int a, int b, int c)
{
	PII key(4*a + b, c);
//	if(Map.find(key) != Map.end())
//		return Map[key];

	int pos = find(a, b);
	if(pos == N)
//		return Map[key] = N;
		return N;

	int i;
	FOR(i, 0, D)
		if(A[pos][i] == c)
//			return Map[key] = find2(pos, i, A[a][b]);
			return find2(pos, i, A[a][b]);

//	return Map[key] = pos;
	return pos;
}

int find2(int a, int b)
{
	int pos = find(a, b);
	if(pos == N)
		return N;

	int i;
	int res = a;
	FOR(i, 0, D)
		res = max(res, find2(pos, i, A[a][b]));

	return res;
}

int SolveTest(int test)
{
	scanf("%d%d%d", &N, &D, &K);

	int i, j;
	FOR(i, 0, N)
		FOR(j, 0, D)
			scanf("%d", &A[i][j]);

	CLEAR(R, -1);
	Map.clear();

	int res = 0;
	int pos = 0;

	if(K == 2)
	{
		FOR(i, 0, N)
			FOR(j, 0, D)
			{
				int a = find2(i, j);
				if(res < a - i)
				{
					res = a - i;
					pos = i;
				}
			}
	}
	else
	{
	}

	printf("Case #%d: %d %d\n", test + 1, pos, pos + res - 1);
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
