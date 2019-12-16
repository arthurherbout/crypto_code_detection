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
int U[1 << 11];
int From[1 << 11];
int S[1 << 11];
vector<PII> E[1 << 11];

VInt f(int edge)
{
	int a = B[edge];
	CLEAR(U, -1);
	U[a] = 0;
	queue<int> q;
	q.push(a);
	while(!q.empty())
	{
		a = q.front();
		q.pop();
		int i;
		FOR(i, 0, SIZE(E[a]))
		{
			int e = E[a][i].second;
			if (e < 0) e = -1 - e;
			if (e == edge) continue;
			int b = E[a][i].first;
			if(U[b] == -1)
			{
				U[b] = 0;
				q.push(b);
				From[b] = E[a][i].second;
			}
		}
	}

	a = A[edge];
	if (U[a] == -1) return VInt();
	VInt res;
	while(a != B[edge])
	{
		int e = From[a];
		res.push_back(e);
		if(e < 0)
		{
			e = -1 - e;
			a = B[e];
		}
		else
		{
			a = A[e];
		}
	}

	res.push_back(edge);
	return res;
}

int SolveTest(int test)
{
	int F, P;
	scanf("%d%d", &F, &P);

	int i, j;
	FOR(i, 0, P)
	{
		int a, b;
		scanf("%d%d", &a, &b);
		--a;
		--b;
		A[i] = a;
		B[i] = b;
		E[a].push_back(PII(b, i));
		E[b].push_back(PII(a, -1 - i));
	}

	vector<VInt> v;
	FOR(i, 0, P)
	{
		VInt r = f(i);
		if (r.empty()) break;
		v.push_back(r);
	}

	FOR(i, 0, F)
		E[i].clear();

	if (SIZE(v) < P)
	{
		printf("Case #%d: IMPOSSIBLE\n", test + 1);
		return 0;
	}

	while(true)
	{
		CLEAR(S, 0);
		int found = 0;
		while(true)
		{
			int zero = 0;
			int inf = 0;
			FOR(i, 0, P)
				if (S[i] < -F*F || F*F < S[i])
				{
					inf = 1;
					break;
				}

			if (inf != 0) break;
			FOR(i, 0, P)
				if(S[i] == 0)
				{
					zero = 1;
					int t = rand() % (2 * F + 1) - F;
					FOR(j, 0, SIZE(v[i]))
					{
						int e = v[i][j];
						if (e < 0) S[-1 - e] -= t;
						else S[e] += t;
					}
				}

			if(zero == 0)
			{
				found = 1;
				break;
			}
		}

		if (found) break;
	}

	printf("Case #%d:", test + 1);
	FOR(i, 0, P)
		printf(" %d", S[i]);
	printf("\n");
	return 0;
}

int main()
{
	freopen("", "r", stdin);
	freopen("", "w", stdout);

	int T, t;
	char buf[1 << 7];
	fgets(buf, 1 << 7, stdin);
	sscanf(buf, "%d", &T);
	FOR(t, 0, T)
	{
		fprintf(stderr, "Solving %d/%d\n", t + 1, T);
		SolveTest(t);
	}

	return 0;
};
