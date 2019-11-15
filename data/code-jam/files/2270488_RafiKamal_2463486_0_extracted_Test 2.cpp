#include <cstdio>
#include <cctype>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <cmath>
#include <stack>
#include <string>     
#include <map>
#include <set>
#include <list>
#include <queue>
#include <utility>

#define LONG_INT __int64
#define SIZE 5005
#define INT_MAX 2000000000
#define MOD 20071027
#define clr(a) memset(a, 0, sizeof a)
#define reset(a) memset(a, -1, sizeof a)

#define BOUNDARY(i, j) ((i >= 0 && i < row) && (j >= 0 && j < column))

#define max3(x, y, z) max(max(x, y), max(y, z)) 

using namespace std;

bool isPalindrome(LONG_INT n)
{
	LONG_INT number = n;
	LONG_INT rev = 0;

	while(number > 0)
	{
		rev = rev * 10 + number % 10;
		number /= 10;
	}
	return n == rev;
}

int main()
{	
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	
	int i, j, k, l;
	int x, y;
	int tc, t;
	int res = 0;
	LONG_INT m, n;
	
	scanf("%d", &tc);

	for(t = 1; t <= tc; t++)
	{
		scanf("%I64d %I64d", &n, &m);
		printf("Case #%d: ", t);

		int a = ceil(sqrt((double) n));
		int b = floor(sqrt((double) m));
		
		res = 0;
		for(i = a; i <= b; i++)
		{
			if(isPalindrome(i) && isPalindrome((LONG_INT) i * (LONG_INT) i))
				res++;
		}
		printf("%d\n", res);	
	}
			
	return 0;
}