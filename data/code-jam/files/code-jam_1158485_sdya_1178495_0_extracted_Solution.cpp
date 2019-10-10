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

using namespace std;

int n;
int a[11000];

void solve(int test)
{
	scanf("%d", &n);
	for (int i = 1; i <= 10000; i ++)
		a[i] = 0;
	for (int i = 1; i <= n; i ++)
	{
		int x;
		scanf("%d", &x);
		a[x] ++;
	}
	int res = 1000000000;

	int cur = 1;
	while (true)
	{
		int start = -1;
		for (int i = cur; i <= 10000; i ++)
			if (a[i] != 0) {start = i; break;}
		if (start == -1) break;

		int x = start + 1;
		while (x <= 10000 && a[x] >= a[x - 1]) x ++;
		x --;
		res = min(res, x - start + 1);
		for (int i = start; i <= x; i ++)
			a[i] --;
	}

	if (res == 1000000000)
		res = 0;
	printf("Case #%d: %d\n", test, res);
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