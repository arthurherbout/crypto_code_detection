/*
 * 2011-06-04  Martin  <Martin@Martin-desktop>

 * 
 */
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>
#include <ctime>

using namespace std;

#ifndef TEMPLATE_BY_TOKEN
//Part1
#define pi 3.1415926535897932384626433832795028841971693993715058209749445923078164062862089986280348253421170679
//Part2
#define ll long long
#define pii pair <int, int>
#define cmplxd complex <double>
//Part3
template <class T> inline T checkmin(T &a, T b)
{
	return (a < b) ? a : a = b;
}

template <class T> inline T checkmax(T &a, T b)
{
	return (a > b) ? a : a = b;
}

template <class T> inline T sqr(T x)
{
	return x * x;
}

template <class T> inline T Lowbit(T x)
{
	return x & (- x);
}

template <class T> T GCD(T a, T b)
{
	if (a < 0)
		return GCD(- a, b);
	if (b < 0)
		return GCD(a, - b);
	return (a == 0) ? b : GCD(b % a, a);
}

template <class T> T LCM(T a, T b)
{
	if (a < 0)
		return LCM(- a, b);
	if (b < 0)
		return LCM(a, - b);
	return (a == 0 || b == 0) ? 0 : a / GCD(a, b) * b;
}

template <class T> T ExtGCD(T a, T b, T &x, T &y)
{
	if (a < 0)
	{
		T c = ExtGCD(- a, b, x, y);
		x = - x;
		return c;
	}
	if (b < 0)
	{
		T c = ExtGCD(a, - b, x, y);
		y = - y;
		return c;
	}
	if (a == 0)
	{
		x = 0, y = 1;
		return b;
	}
	else
	{
		T c = ExtGCD(b % a, a, y, x);
		x -= b / a * y;
		return c;
	}
}
#endif

#define rep(i, n) for (int i = 0; i < n; ++ i)
#define forvector(i, v) for (int i = 0; i < (int) (v).size(); ++ i)
#define pb push_back
#define mp make_pair
#define x first
#define y second
#define MaxiN 1000005

bool isPrime[MaxiN];
vector <int> Prime;

void PreWork()
{
	memset(isPrime, 1, sizeof(isPrime));
	isPrime[0] = isPrime[1] = 0;
	rep (i, MaxiN)
		if (isPrime[i])
			for (int j = 2; i * j < MaxiN; ++ j)
				isPrime[i * j] = 0;
	Prime.clear();
	rep (i, MaxiN)
		if (isPrime[i])
			Prime.pb(i);
}

ll solve()
{
	ll N, ans = 0;
	cin >> N;
	if (N == 1)
		return 0;
	ans = 1;
	forvector (i, Prime)
	{
		if ((ll) Prime[i] * (ll) Prime[i] > N)
			break;
		ll tmp = N / Prime[i];
		while (tmp >= Prime[i])
			tmp /= Prime[i], ++ ans;
	}
	return ans;
}

int main()
{
	PreWork();
	int TestCase;
	scanf("%d", &TestCase);
	rep (i, TestCase)
		printf("Case #%d: %lld\n", i + 1, solve());
}
