#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <stack>
#include <utility>
#include <string.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cctype>

using namespace std;

typedef unsigned int uint;
typedef long long ll;
typedef long lint;

#define MP make_pair
#define PB push_back
#define EPS 0.0000001
#define ALL(a) a.begin(), a.end()

const int INF = 1000 * 1000 * 1000;
const int MOD = 1000 * 1000 * 1000 + 7;
const lint INF_L = 1000 * 1000 * 1000;
const ll INF_LL = 1LL * INF_L * INF_L;
const ll MOD_LL = 1000000000007LL;


ll gcd(ll a, ll b)
{
	while (b)
	{
		a %= b;
		swap(a, b);
	}
	return a;
}

ll lcm(ll a, ll b)
{
	return (a / gcd(a, b)) * b;
}

int c, d, v;
set <int> ds;
set <int> vs;
set <int> sums;
int minn;

void cnt(int rest, vector <bool> &u, int sum)
{
	if (!rest)
	{
		sums.insert(sum);
		vs.erase(sum);
		return;
	}
	auto it = ds.begin();
	for (int i = 0; i < ds.size(); ++i, ++it)
	{
		if (!u[i])
		{
			u[i] = true;
			cnt(rest - 1, u, sum + *(it));
			u[i] = false;
		}
	}
}

void allSums()
{
	for (int i = 1; i <= ds.size(); ++i)
	{
		vector <bool> u(ds.size(), false);
		cnt(i, u, 0);
	}
}


//#define ONLINE_JUDGE
int main()
{
#ifndef ONLINE_JUDGE
	freopen("C-small-attempt1.in", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif

	int t;
	cin >> t;
	for (int tt = 1; tt <= t; ++tt)
	{
		cin >> c >> d >> v;
		ds.clear();
		for (int i = 0; i < d; ++i)
		{
			int a;
			cin >> a;
			ds.insert(a);
		}
		vs.clear();
		for (int i = 1; i <= v; ++i)
			vs.insert(i);
		sums.clear();
		allSums();
		while (vs.size())
		{
			map <int, int> m;
			for (auto i : vs)
			{
				for (auto j : sums)
				{
					if (i - j > 0)
					{
						m[i - j]++;
					}
				}
			}
			int maxx = -1;
			int cur = -1;
			for (auto i : m)
			{
				if ((maxx == -1 || i.second > maxx) && !ds.count(i.first))
				{
					maxx = i.second;
					cur = i.first;
				}
			}
			if (cur == -1)
			{
				cur = *vs.begin();
			}
			vs.erase(cur);
			for (auto i : sums)
			{
				vs.erase(i + cur);
			}
			set <int> newsums;
			newsums.insert(cur);
			for (auto i : sums)
			{
				newsums.insert(i + cur);
			}
			sums.insert(newsums.begin(), newsums.end());
			ds.insert(cur);
		}

		printf("Case #%d: %d\n", tt, ds.size() - d);
	}

	return 0;
}