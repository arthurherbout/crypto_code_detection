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

Int A[1 << 20];

int SolveTest(int test)
{
	int N, p, q, r, s;
	scanf("%d%d%d%d%d", &N, &p, &q, &r, &s);

	int i;
	Int sum = 0;
	FOR(i, 0, N)
	{
		Int t = i;
		t = (t * p + q) % r + s;
		A[i] = t;
		sum += t;
	}

	int left = 0;
	int right = N - 1;
	Int lSum = 0;
	Int rSum = 0;
	Int res = sum;
	while (left != right)
	{
		if (lSum + A[left] < rSum + A[right])
		{
			lSum += A[left];
			++left;
		}
		else
		{
			rSum += A[right];
			--right;
		}

		res = min(res, max(sum - lSum - rSum, max(lSum, rSum)));
	}

	printf("Case #%d: %.11lf\n", test + 1, (sum - res + 0.0) / sum);

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
