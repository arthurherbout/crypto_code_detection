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

int S[1 << 20];

int SolveTest(int test)
{
	int N, K;
	scanf("%d%d", &N, &K);

	int i, j;
	FOR(i, 0, N - K + 1)
		scanf("%d", &S[i]);

	VInt v;
	int sum = S[0];
	FOR(i, 0, K)
	{
		int s = 0;
		int Min = 0;
		int Max = 0;

		for (j = i + K; j < N; j += K)
		{
			s += S[j - K + 1] - S[j - K];
			Min = min(Min, s);
			Max = max(Max, s);
		}

		sum += Min;
		v.push_back(Max - Min);
	}

	sort(ALL(v));
	int len = v.back();

	int r = 0;
	FOR(i, 0, K)
		r += len - v[i];

	int times = sum / K;
	if (times * K > sum)
		--times;

	if (sum - times * K > r)
		++len;

	printf("Case #%d: %d\n", test + 1, len);
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
