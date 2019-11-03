#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <bitset>
#include <queue>
#include <math.h>

using namespace std;

char s[110];
vector < long long > pos;

bool is_square(long long n)
{
	long long m = (long long)(sqrt((double)(n)));
	for (long long i = -1LL; i <= 1LL; i ++)
		if ((m + i) * (m + i) == n) return true;
	return false;
}

void solve(int test)
{
	gets(s);
	pos.clear();
	int n = strlen(s);
	for (int i = 0; i < n; i ++)
		if (s[i] == '?') pos.push_back(n - 1 - i);
	long long cur = 0;
	for (int i = 0; i < n; i ++)
		if (s[i] == '?') cur *= 2LL; else cur = cur * 2LL + (long long)(s[i] - '0');

	int m = pos.size();
	for (int i = 0; i < (1 << m); i ++)
	{
		long long add = 0;
		for (int j = 0; j < m; j ++)
			if (i & (1 << j)) add += (1LL << pos[j]);
		if (is_square(cur + add))
		{
			printf("Case #%d: ", test);
			for (int j = 0; j < m; j ++)
				if (i & (1 << j)) s[n - 1 - pos[j]] = '1'; else s[n - 1 - pos[j]] = '0';
			puts(s);
			return ;
		}
	}
}

int main()
{
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	int test;
	scanf("%d\n", &test);
	for (int i = 1; i <= test; i ++)
	{
		cerr << i << endl;
		solve(i);
	}
	return 0;
}