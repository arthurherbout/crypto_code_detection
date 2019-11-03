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

long long n;
int a[50];
int mx = 0, mn = 1000000000;

int gcd(int a, int b)
{
	if (a == 0 || b == 0) return a + b;
	if (a > b) return gcd(a % b, b);
	return gcd(b % a, a);
}

int lcm(int a, int b)
{
	return (a / gcd(a, b)) * b;
}

void rec(int cur, int cost, int moves)
{
	if (cur == n)
	{
		mx = max(moves, mx);
		mn = min(moves, mn);
		return ;
	}

	if (cost == 0)
	{
		cost = a[cur + 1];
		rec(cur + 1, a[cur + 1], moves + 1);
		return ;
	}
	if (cost % a[cur + 1] != 0)
		rec(cur + 1, lcm(cost, a[cur + 1]), moves + 1); else
		rec(cur + 1, cost, moves);
}

bool primes[1100000];

void solve(int test)
{
	scanf("%lld", &n);
	long long res = 0;
	if (n == 1) res = 0; else
	{
		res = 1;
		for (long long i = 2; i * i <= n; i ++)
			if (!primes[i])
			{
				long long s = i * i;
				while (s <= n) res ++, s *= i;
			}
	}

	printf("Case #%d: %lld\n", test, res);
}

void precalc()
{
	primes[1] = true;
	for (int i = 2; i <= 1000000; i ++)
		if (!primes[i])
			for (int j = i + i; j <= 1000000; j += i)
				primes[j] = true;
}

int main()
{
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	precalc();
	int test;
	scanf("%d\n", &test);
	for (int i = 1; i <= test; i ++)
		solve(i);
	return 0;
}