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

int gcd(int a, int b){ return a == 0 ? b : gcd(b % a, a); }

PII egcd(int a, int b, int c)
{
	if(b == 0)
		return PII(c, 0);

	PII res = egcd(b, a % b, c);
	int x = res.second;
	int y = res.first - (a/b)*res.second;
	int z = y % a;
	x += z*b;
	y -= z*a;

	return PII(x, y);
}

int main()
{
	freopen("B.in", "r", stdin);
	freopen("B.out", "w", stdout);

	int T, t;
	scanf("%d", &T);
	FOR(t, 0, T)
	{
		int N, M, K;
		scanf("%d%d%d", &N, &M, &K);

		int found = 0;
		int i, j, ii, jj;
/*		FOR(i, 1, N + 1)
		{
			int from = (K + i - 1) / i;
			FOR(j, from, M + 1)
			{
				int g = gcd(i, j);
				if(K % g)
					continue;

				PII temp = egcd(i / g, j / g, K / g);

				int a = temp.first;
				int b = temp.second;
				if(b < 0)
				{
					b += i / g;
					a -= j / g;
				}

				if(a <= 0)
				{
					if(i*a + b*j != K)
						throw 0;

					printf("Case #%d: %d %d %d %d %d %d\n", t + 1, 0, 0, i, j, b, -a);
					found = 1;
					break;
				}
			}

			if(found)
				break;
		}*/

		FOR(i, 0, N + 1)
			if(found == 0)
			FOR(j, 0, M + 1)
				if(found == 0)
				FOR(ii, 0, N + 1)
					if(found == 0)
					FOR(jj, 0, M + 1)
						if(found == 0)
						if(abs(i*jj - ii*j) == K)
						{
							found = 1;
							printf("Case #%d: %d %d %d %d %d %d\n", t + 1, 0, 0, i, j, ii, jj);
							break;
						}

		if(found == 0)
			printf("Case #%d: IMPOSSIBLE\n", t + 1);
	}



	return 0;
};
