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

VInt A[10];
vector<VInt> E[10];
int B[1 << 16];
int C[10][10];

int f(int n, int a)
{
	int pos = lower_bound(ALL(A[n]), a) - A[n].begin();
	if (pos < 0 || SIZE(A[n]) <= pos || A[n][pos] != a) return 1;

	CLEAR(B, 0);
	queue<int> q;
	q.push(pos);
	B[pos] = 1;
	int res = 0;
	while(!q.empty())
	{
		pos = q.front();
		q.pop();

		int i;
		a = A[n][pos];
		int s = 0;
		int left = n;
		int r = 1;
		RFOR(i, n + 1, 1)
		{
			int d = a % 10;
			r *= C[left][d];
			left -= d;
			s += d*i;
			a /= 10;
		}

		if (s > n) res += r;
		++res;

		FOR(i, 0, SIZE(E[n][pos]))
		{
			int b = E[n][pos][i];
			if(B[b] == 0)
			{
				B[b] = 1;
				q.push(b);
			}
		}
	}

	return res;
}

int SolveTest(int test)
{
	int a = 0;
	char buf[16];
	scanf("%s", buf);
	int cnt = strlen(buf);
	int i;
	FOR(i, 0, cnt)
		a = a * 10 + buf[i] - '0';

	printf("Case #%d: %d\n", test + 1, f(cnt, a));
	return 0;
}

int main()
{
	int i, j;
	int limit = 1;
	FOR(i, 1, 10)
	{
		limit *= 10;
		FOR(j, 1, limit)
		{
			int a = j;
			int s = 0;
			while (a != 0)
			{
				int d = a % 10;
				if (d > i) break;
				s += d;
				if (s > i) break;
				a /= 10;
			}
			if (a == 0)
				A[i].push_back(j);
		}

		int cnt = SIZE(A[i]);
		E[i] = vector<VInt>(cnt, VInt());
		fprintf(stderr, "cnt[%d] = %d\n", i, cnt);
		FOR(j, 0, cnt)
		{
			int a = A[i][j];
			int d[10];
			CLEAR(d, 0);
			while (a != 0)
			{
				++d[a % 10];
				a /= 10;
			}

			int k;
			int t = 0;
			FOR(k, 1, i + 1)
				t = t * 10 + d[k];

			int pos = lower_bound(ALL(A[i]), t) - A[i].begin();
			if (A[i][pos] != t) throw 0;
			E[i][pos].push_back(j);
		}
	}

	CLEAR(C, 0);
	FOR(i, 0, 10) C[i][i] = C[i][0] = 1;
	FOR(i, 0, 10)
		FOR(j, 1, i)
		C[i][j] = C[i - 1][j] + C[i - 1][j - 1];

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
