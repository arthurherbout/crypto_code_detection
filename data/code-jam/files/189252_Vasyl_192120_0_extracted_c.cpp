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

int A[128];
int Res[128][128];

int F(int from, int to)
{
	if(to - from <= 1)
		return 0;
	int& res = Res[from][to];
	if(res != -1)
		return res;

	res = (to - from - 1)*(A[to] - A[from] - 1);
	int i;
	FOR(i, from + 1, to)
		res = min(res, F(from, i) + F(i, to) + A[to] - A[from] - 2);

	return res;
}

int SolveTest(int test)
{
	int N, M;
	scanf("%d%d", &N, &M);

	int i;
	FOR(i, 0, M)
		scanf("%d", &A[i]);

	A[M] = 0;
	A[M + 1] = N + 1;
	sort(A, A + M + 2);
	CLEAR(Res, -1);
	
	printf("Case #%d: %d\n", test + 1, F(0, M + 1));
	return 0;
}

int main()
{
	freopen("c.in", "r", stdin);
	freopen("c.out", "w", stdout);

	char buf[1 << 10];
	gets(buf);
	int T, t;
	sscanf(buf, "%d", &T);
	FOR(t, 0, T)
		SolveTest(t);

	return 0;
};
