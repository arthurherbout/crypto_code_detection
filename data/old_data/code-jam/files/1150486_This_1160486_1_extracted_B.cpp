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
#define MaxiN 505

int X[MaxiN][MaxiN];
int Y[MaxiN][MaxiN];
int A[MaxiN][MaxiN];

int getS(int (*S)[MaxiN], int x1, int x2, int y1, int y2)
{
	return S[x2][y2] - S[x1 - 1][y2] - S[x2][y1 - 1] + S[x1 - 1][y1 - 1];
}

double solve()
{
	int R, C, D, ans = 1;
	scanf("%d%d%d", &R, &C, &D);
	for (int i = 1; i <= R; ++ i)
		for (int j = 1; j <= C; ++ j)
		{
			char Ch = getchar();
			while (!isdigit(Ch))
				Ch = getchar();
			A[i][j] = Ch - '0';
			X[i][j] = i * (Ch - '0');
			Y[i][j] = j * (Ch - '0');
		}
	for (int i = 1; i <= R; ++ i)
		for (int j = 1; j <= C; ++ j)
		{
			A[i][j] += A[i - 1][j] + A[i][j - 1] - A[i - 1][j - 1];
			X[i][j] += X[i - 1][j] + X[i][j - 1] - X[i - 1][j - 1];
			Y[i][j] += Y[i - 1][j] + Y[i][j - 1] - Y[i - 1][j - 1];
		}
	for (int i = 1; i <= R; ++ i)
		for (int j = 1; j <= C; ++ j)
			for (int k = (ans - 1) / 2 + 1; i - k >= 1 && i + k <= R && j - k >= 1 && j + k <= C; ++ k)
			{
				int sl = getS(Y, i - k, i + k, j - k, j - 1) - getS(Y, i - k, i - k, j - k, j - k) - getS(Y, i + k, i + k, j - k, j - k);
				int sr = getS(Y, i - k, i + k, j + 1, j + k) - getS(Y, i - k, i - k, j + k, j + k) - getS(Y, i + k, i + k, j + k, j + k);
				int su = getS(X, i - k, i - 1, j - k, j + k) - getS(X, i - k, i - k, j - k, j - k) - getS(X, i - k, i - k, j + k, j + k);
				int sd = getS(X, i + 1, i + k, j - k, j + k) - getS(X, i + k, i + k, j - k, j - k) - getS(X, i + k, i + k, j + k, j + k);
				int al = getS(A, i - k, i + k, j - k, j - 1) - getS(A, i - k, i - k, j - k, j - k) - getS(A, i + k, i + k, j - k, j - k);
				int ar = getS(A, i - k, i + k, j + 1, j + k) - getS(A, i - k, i - k, j + k, j + k) - getS(A, i + k, i + k, j + k, j + k);
				int au = getS(A, i - k, i - 1, j - k, j + k) - getS(A, i - k, i - k, j - k, j - k) - getS(A, i - k, i - k, j + k, j + k);
				int ad = getS(A, i + 1, i + k, j - k, j + k) - getS(A, i + k, i + k, j - k, j - k) - getS(A, i + k, i + k, j + k, j + k);
				if (al * j - sl == sr - ar * j && au * i - su == sd - ad * i)
					ans = k * 2 + 1;
			}
	for (int i = 1; i <= R; ++ i)
		for (int j = 1; j <= C; ++ j)
			for (int k = ans / 2 + 1; i - k >= 0 && i + k <= R && j - k >= 0 && j + k <= C; ++ k)
			{
				int sl = getS(Y, i - k + 1, i + k, j - k + 1, j) - getS(Y, i - k + 1, i - k + 1, j - k + 1, j - k + 1) - getS(Y, i + k, i + k, j - k + 1, j - k + 1);
				int sr = getS(Y, i - k + 1, i + k, j + 1, j + k) - getS(Y, i - k + 1, i - k + 1, j + k, j + k) - getS(Y, i + k, i + k, j + k, j + k);
				int su = getS(X, i - k + 1, i, j - k + 1, j + k) - getS(X, i - k + 1, i - k + 1, j - k + 1, j - k + 1) - getS(X, i - k + 1, i - k + 1, j + k, j + k);
				int sd = getS(X, i + 1, i + k, j - k + 1, j + k) - getS(X, i + k, i + k, j - k + 1, j - k + 1) - getS(X, i + k, i + k, j + k, j + k);
				int al = getS(A, i - k + 1, i + k, j - k + 1, j) - getS(A, i - k + 1, i - k + 1, j - k + 1, j - k + 1) - getS(A, i + k, i + k, j - k + 1, j - k + 1);
				int ar = getS(A, i - k + 1, i + k, j + 1, j + k) - getS(A, i - k + 1, i - k + 1, j + k, j + k) - getS(A, i + k, i + k, j + k, j + k);
				int au = getS(A, i - k + 1, i, j - k + 1, j + k) - getS(A, i - k + 1, i - k + 1, j - k + 1, j - k + 1) - getS(A, i - k + 1, i - k + 1, j + k, j + k);
				int ad = getS(A, i + 1, i + k, j - k + 1, j + k) - getS(A, i + k, i + k, j - k + 1, j - k + 1) - getS(A, i + k, i + k, j + k, j + k);
				if ((ll) al * (ll) (j * 2 + 1) - (ll) sl * 2 == (ll) sr * 2 - (ll) ar * (ll) (j * 2 + 1) && (ll) au * (ll) (i * 2 + 1) - (ll) su * 2 == (ll) sd * 2 - (ll) ad * (ll) (i * 2 + 1))
					ans = k * 2;
			}
	return ans;
}

int main()
{
	int TestCase;
	scanf("%d", &TestCase);
	rep (i, TestCase)
	{
		int res = solve();
		if (res < 3)
			printf("Case #%d: IMPOSSIBLE\n", i + 1);
		else
			printf("Case #%d: %d\n", i + 1, res);
	}
}
