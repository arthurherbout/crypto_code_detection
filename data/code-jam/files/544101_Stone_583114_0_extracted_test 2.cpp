#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <iterator>
#include <algorithm>
#include <queue>
#include <functional>
#include <sstream>
#include <complex>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <ctime>
#include <iomanip>
#include <time.h>

using namespace std;


#ifdef ONLINE_JUDGE
void init()
{
}
#else
FILE* inputstream;
FILE* outputstream;
void init()
{
	inputstream = freopen("in.txt", "r", stdin);
	outputstream = freopen("output.txt", "w", stdout);
}
#endif

map<pair<int, int>, bool> mp;

bool go(int a, int b)
{
	if (a <= 0 || b <= 0)
	{
		return true;
	}
	if (mp.count(make_pair(a, b)) != 0)
	{
		return mp[make_pair(a, b)];
	}
	bool& bo = mp[make_pair(a, b)];
	bo = false;
	if (!go(b, a % b)) bo = true;
	if (a % b + b < a && !go(a % b + b, b)) bo = true;
	if (a % b + 2 * b < a && !go(a % b + 2 * b, b)) bo = true;
	return bo;
}

int main()
{
	init();
	int cases;
	scanf("%d", &cases);
	int a1, a2, b1, b2;
	for (int i = 1; i <= cases; ++i)
	{
		scanf("%d %d %d %d", &a1, &a2, &b1, &b2);
		int suc = 0;
		for (int j = a1; j<= a2; ++j)
		{
			for (int k = b1; k <= b2; ++k)
			{
				mp.clear();
				int big = max(j, k);
				int sml = min(j, k);
				if (go(big, sml))
				{
					//cout << j << " " << k << endl;
					++suc;
				}
			}
		}
		printf("Case #%d: %d\n", i, suc);
	}
	return 0;
}
