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
	const int MaxiN = 1005;
	
	int N;
	int A[MaxiN], B[MaxiN], V[MaxiN];
	
	string ToString(int X)
	{
		stringstream SS;
		SS << X;
		return SS.str();
	}
	
	string Solve()
	{
		scanf("%d", &N);
		rep (i, N)
			scanf("%d%d", A + i, B + i);
		fill(V, V + N, 0);
		int res = 0;
		int Star = 0;
		for (bool Flag = 1; Flag; )
		{
			Flag = 0;
			rep (i, N)
			{
				if (V[i] == 0 && B[i] <= Star)
				{
					Star += 2;
					Flag = 1;
					V[i] = 2;
					++ res;
				}
				if (V[i] == 1 && B[i] <= Star)
				{
					++ Star;
					Flag = 1;
					V[i] = 2;
					++ res;
				}
			}
			if (!Flag)
			{
				int Max = INT_MIN;
				int k = - 1;
				rep (i, N)
					if (V[i] == 0 && A[i] <= Star && B[i] > Max)
					{
						Max = B[i];
						k = i;
					}
				if (k != - 1)
				{
					++ Star;
					V[k] = 1;
					Flag = 1;
					++ res;
				}
			}
		}
		if (Star == N * 2)
			return ToString(res);
		return "Too Bad";
	}
	
	void Run()
	{
		int TestCase = 0;
		scanf("%d", &TestCase);
		for (int T = 1; T <= TestCase; ++ T)
			printf("Case #%d: %s\n", T, Solve().c_str());
	}
}

int main()
{
	Poor::Run();
	return 0;
}
