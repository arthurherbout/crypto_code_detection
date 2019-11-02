/*
 * 2012-04-28  Martin  <Martin@Martin-desktop>

 * 
 */
#include <climits>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdarg>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <memory>
#include <locale>
#include <bitset>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>
#include <string>
#include <complex>
#include <valarray>

using namespace std;

template <class T> inline T checkmin(T &a, T b)
{
	return (a < b) ? a : a = b;
}

template <class T> inline T checkmax(T &a, T b)
{
	return (a > b) ? a : a = b;
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

#define tr(i, x) for (typeof(x.begin()) i = x.begin(); i != x.end(); ++ i)
#define rep(i, n) for (int i = 0; i < n; ++ i)
#define pii pair <int, int>
#define mp make_pair
#define pb push_back
#define x first
#define y second
#define ll long long

namespace Poor
{
	const int MaxiA = 100005;
	
	int A, B;
	double P[MaxiA], S[MaxiA];
	
	double Solve()
	{
		scanf("%d%d", &A, &B);
		rep (i, A)
			scanf("%lf", P + i);
		S[0] = 1.0;
		rep (i, A)
			S[i + 1] = S[i] * P[i];
		double res = B + 2;
		for (int i = 0; i <= A; ++ i)
			checkmin(res, 1.0 * (double) (B - A + i + i + 1) + (1.0 - S[A - i]) * (double) (B + 1));
		return res;
	}
	
	void Run()
	{
		int TestCase = 0;
		scanf("%d", &TestCase);
		for (int T = 1; T <= TestCase; ++ T)
			printf("Case #%d: %.12lf\n", T, Solve());
	}
}

int main()
{
	Poor::Run();
	return 0;
}
