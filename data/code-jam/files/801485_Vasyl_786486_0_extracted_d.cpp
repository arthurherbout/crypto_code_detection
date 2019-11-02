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

#define MAX 4782969

Int X[32];
pair<int, Int> A[MAX];
pair<int, Int> B[MAX];

int Generate(Int X[], int cnt, pair<int, Int> A[], int sum, Int left)
{
	if(left < 0)
		return 0;

	if(cnt == 0)
	{
		A[0] = MP(sum, left);
		return 1;
	}

	Int dist = X[0] - X[1];
	if(dist < 0)
		dist = -dist;

	int res = 0;
	int i;
	FOR(i, -1, 2)
		if(sum + i > 0)
			res += Generate(X + 1, cnt - 1, A + res, sum + i, left - 2*dist*(sum + i));

	return res;
}

int SolveTest(int test)
{
	int N;
	Int x;
	scanf("%d%lld", &N, &x);
	--N;

	int i, j, k;
	FOR(i, 0, N)
		scanf("%lld", &X[i]);

	Int fuel;
	scanf("%lld", &fuel);

	if(N == 1)
	{
		X[N] = 0;
		++N;
	}

	sort(X, X + N);

	if(X[0] > 0)
		X[0] = 0;
	if(X[N - 1] < 0)
		X[N - 1] = 0;

	int cntA = N/2;
	int cntB = N - cntA;

	cntA = Generate(X, cntA, A, 0, fuel);
	reverse(X, X + N);
	cntB = Generate(X, cntB, B, 0, fuel);
	reverse(X, X + N);

	sort(A, A + cntA);
	sort(B, B + cntB);

	Int res = fuel;

	Int limit = fuel - 2*(X[N/2] - X[N/2 - 1]);
	int prev = 0;
	j = 0;
	FOR(i, 1, cntA + 1)
		if(i == cntA || A[i].first != A[i - 1].first)
		{
			while(j != cntB && B[j].first < A[prev].first)
				++j;

			RFOR(k, i, prev)
			{
				while(j != cntB && B[j].first == A[k].first && B[j].second + A[k].second < limit)
					++j;

				if(j != cntB && B[j].first == A[k].first && B[j].second + A[k].second >= limit)
					res = min(res, B[j].second + A[k].second - limit);
			}

			prev = i;
		}

	if(res == fuel)
		printf("Case #%d: NO SOLUTION\n", test + 1);
	else
		printf("Case #%d: %lld\n", test + 1, fuel - res);

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
