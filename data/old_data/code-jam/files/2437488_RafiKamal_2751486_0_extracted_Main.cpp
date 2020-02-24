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
#include <sstream>
#include <iomanip>
#include <fstream>

#define SIZE 5000
#define MOD 20071027
#define clr(a) memset(a, 0, sizeof a)
#define reset(a) memset(a, -1, sizeof a)

#define BOUNDARY(i, j) ((i >= 0 && i < row) && (j >= 0 && j < column))

#define max3(x, y, z) max(max(x, y), max(y, z)) 

using namespace std;

int n, m, row, column, mx;

bool isConsonant(char c)
{
	return c != 'a' && c != 'e' && c  != 'i' && c != 'o' && c != 'u';
}

int main()
{	
	freopen("input.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	int i, j, k, m, p, q, tc, t;
	
	scanf("%d", &tc);
	for(t = 1; t <= tc; t++)
	{
		string str;
		cin >> str >> n;
		long long cnt = 0;
		
		for(i = 0; i < str.size(); i++)
		{
			for(j = i + n - 1; j < str.size(); j++)
			{
				for(k = 0; k < n; k++) {
					if(!isConsonant(str[j - k])) break;
				}
				if(k == n) {
					cnt += (str.length() - j);
					break;
				}
			}
		}
		
		printf("Case #%d: ", t);
		cout << cnt << endl;
	}
			   
	return 0;
}