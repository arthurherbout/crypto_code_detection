// ThemePark.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <cstring>
#include <string.h>
#include <map>
#include <deque>
#include <queue>
#include <stack>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <set>
#include <complex>
#include <list>

using namespace std;

//#define SMALL
#define LARGE
struct TSnapper {
	bool bPower;
	bool bOn;
};

long long gQ[1000];

void getOnBoard(int next, long long k, long long N, long long total, int *cacheNext, long long *cacheOnboard)
{
	int onBoard = 0;
	int orgNext = next;
	while (true)
	{
		long long t = gQ[next] + onBoard;
		if (t > k || t > total)
			break;

		onBoard += gQ[next];
		next++;
		if (next >= N)
			next = 0;
	}
	cacheNext[orgNext] = next;
	cacheOnboard[orgNext] = onBoard;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//freopen("c.txt", "rt", stdin);
#ifdef SMALL
	freopen("C-small-attempt0.in","rt",stdin);
	freopen("C-small.out","wt",stdout);
#endif
#ifdef LARGE
	freopen("C-large.in","rt",stdin);
	freopen("C-large.out","wt",stdout);
#endif
	int T;
	cin >> T;
	for (int i = 1; i <= T; i++)
	{
		printf("Case #%d: ", i);

		long long R, k;
		int N;
		cin >> R >> k >> N;
		long long total = 0;
		for (int i=0; i<N; i++)
		{
			cin >> gQ[i]; // assume no group > k
			total += gQ[i];
		}
		long long money = 0;
		long long repMoney = 0;
		long long repCount = 0;
		int next = 0;
		long long cacheOnboard[1000] = {0};
		int cacheNext[1000] = {0};

		for (long long r=0; r<R; r++)
		{
			// start
			if (cacheOnboard[next] == 0 && cacheNext[next] == 0)
				getOnBoard(next, k , N, total, cacheNext, cacheOnboard);

			money += cacheOnboard[next];
			next = cacheNext[next];
			if (next == 0)
			{
				repMoney = money;
				repCount = R/(r+1);
				r = repCount * (r+1)-1;
				money = repCount * money;
			}
		}
		cout << money << endl;
	}
	return 0;
}