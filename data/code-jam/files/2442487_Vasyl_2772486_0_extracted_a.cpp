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

#define MOD 1000002013

int A[1 << 20];
int B[1 << 20];
Int C[1 << 20];
int D[1 << 20];

int SolveTest(int test)
{
	int n, m;
	scanf("%d%d", &n, &m);

	int i;
	FOR(i, 0, m)
	{
		int a, b, c;
		scanf("%d%d%d", &a, &b, &c);
		A[i] = a;
		B[i] = b;
		C[i] = c;
	}

	CLEAR(D, 0);
	Int res = 0;
	while(true)
	{
		int time = n + 1;
		int pos = -1;
		FOR(i, 0, m)
			if(D[i] == 0 && B[i] < time)
			{
				time = B[i];
				pos = i;
			}

		if(pos == -1)
			break;

		D[pos] = 1;
		vector<PII> v;
		FOR(i, 0, m)
			if(D[i] == 0 && A[i] <= B[pos])
				v.PB(PII(A[i], i));

		sort(ALL(v));
		RFOR(i, SIZE(v), 0)
		{
			if(C[pos] == 0)
				break;
			int a = v[i].second;
			if(A[a] <= A[pos])
				break;

			Int c = (A[a] - A[pos])*(B[a] - B[pos]) % MOD;
			if(C[a] <= C[pos])
			{
				res += C[a]*c;
				res %= MOD;

				C[pos] -= C[a];
				A[a] = A[pos];
			}
			else
			{
				res += C[pos]*c;
				res %= MOD;

				B[pos] = B[a];
				C[a] -= C[pos];
				D[pos] = 0;
				break;
			}
		}
	}

	printf("Case #%d: %d\n", test + 1, int(res));
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
