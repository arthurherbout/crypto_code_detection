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

int A[1 << 14];
int B[1 << 14];
int C[1 << 14];

int SolveTest(int test)
{
	int N;
	scanf("%d", &N);

	int i, j, k;
	FOR(i, 0, N)
		scanf("%d", &A[i]);

	sort(A, A + N);

	if(N == 0)
	{
		printf("Case #%d: 0\n", test + 1);
		return 0;
	}

	int Min = 0, Max = N + 1;
	while(Max - Min > 1)
	{
		int Mid = (Max + Min) >> 1;
		CLEAR(C, 0);
		CLEAR(B, 0);
		FOR(i, 0, N)
			++C[ A[i] ];

		FOR(i, 1, 1 << 14)
		{
			while(C[i] != 0)
			{
				if(B[i - 1] != 0)
				{
					--C[i];
					--B[i - 1];
					++B[i];
					continue;
				}

				FOR(k, 0, Mid)
				{
					if(C[i + k] == 0)
						break;

					--C[i + k];
				}

				if(k < Mid)
				{
					++C[i];
					break;
				}

				++B[i + Mid - 1];
			}

			if(C[i] != 0)
				break;
		}

		if(i == (1 << 14))
			Min = Mid;
		else
			Max = Mid;
	}

	printf("Case #%d: %d\n", test + 1, Min);

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
