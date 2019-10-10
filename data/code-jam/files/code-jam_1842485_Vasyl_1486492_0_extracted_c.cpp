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

int A[1 << 12];
int R[1 << 12];

int F(int first, int last, int delta)
{
	if(first == last)
		return 1;

	R[first] = R[last] - delta*(last - first);

	if(A[first] > last)
		return 0;
	if(A[first] == last)
		return F(first + 1, last, delta + 1);

	if(F(A[first], last, delta) == 0)
		return 0;

	return F(first + 1, A[first], delta + 1);
}

int SolveTest(int test)
{
	int n;
	scanf("%d", &n);

	int i;
	FOR(i, 0, n - 1)
		scanf("%d", &A[i]);

	FOR(i, 0, n - 1)
		--A[i];

	R[n - 1] = 999999999;
	printf("Case #%d:", test + 1);
	if(F(0, n - 1, 0) == 0)
		printf(" Impossible");
	else
		FOR(i, 0, n)
			printf(" %d", R[i]);

	printf("\n");
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
