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

int A[1 << 11];
int B[1 << 11];
int C[1 << 11];
int D[1 << 11];
int R[1 << 11];
int S[1 << 11];

int SolveTest(int test)
{
	int n;
	scanf("%d", &n);

	int i, j;
	FOR(i, 0, n)
		scanf("%d", &A[i]);
	FOR(i, 0, n)
		scanf("%d", &B[i]);

	CLEAR(D, 0);
	FOR(i, 0, n)
	{
		int prev = 0;
		CLEAR(C, 0);

		S[0] = 0;
		FOR(j, 0, n)
		{
			S[j + 1] = S[j];
			if(D[j] == 1)
				S[j + 1] = max(S[j + 1], A[j]);
		}

		prev = n + 1;
		RFOR(j, n, 0)
			if(D[j] == 0 && A[j] == S[j] + 1 && prev > A[j])
			{
				prev = A[j];
				C[j] |= 1;
			}

		S[n] = 0;
		RFOR(j, n, 0)
		{
			S[j] = S[j + 1];
			if(D[j] == 1)
				S[j] = max(S[j], B[j]);
		}

		prev = n + 1;
		FOR(j, 0, n)
			if(D[j] == 0 && B[j] == S[j] + 1 && prev > B[j])
			{
				prev = B[j];
				C[j] |= 2;
			}

		FOR(j, 0, n)
			if(C[j] == 3)
				break;

		D[j] = 1;
		R[j] = i + 1;
	}

	printf("Case #%d: ", test + 1);
	FOR(i, 0, n)
		printf("%d%c", R[i], i == n - 1 ? '\n' : ' ');

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
