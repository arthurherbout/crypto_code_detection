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

char S[128];

int SolveTest(int test)
{
	scanf("%s", S);

	int len = strlen(S);
	int i, j;
	int cnt = 0;
	FOR(i, 0, len)
		if(S[i] == '?')
			++cnt;

	Int res = 0;
	FOR(i, 0, 1 << cnt)
	{
		int t = i;
		Int r = 0;
		FOR(j, 0, len)
			if(S[j] == '?')
			{
				r <<= 1;
				r += t & 1;
				t >>= 1;
			}
			else
			{
				r <<= 1;
				r += S[j] - '0';
			}

		Int Min = 0, Max = 1 << 30;
		while(Max - Min > 1)
		{
			Int Mid = (Max + Min) >> 1;
			if(Mid*Mid > r)
				Max = Mid;
			else
				Min = Mid;
		}

		if(Min*Min == r)
			res = Min;
	}

	string ret = "";
	res *= res;
	while(res != 0)
	{
		ret.PB('0' + (res & 1));
		res >>= 1;
	}

	reverse(ALL(ret));

	printf("Case #%d: %s\n", test + 1, ret.c_str());
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
