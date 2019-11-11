#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <string>
#include <cassert>
#include <set>

using namespace std;

#define MAX_INT 100000

int tab[11];
int used[11][MAX_INT];

bool solve(int x)
{
	for (int i = 2; i <= 10; ++i)
	{
		if (tab[i] == 0) continue;
		set<int> s;

		int w = x;
		while (w != 1)
		{
			if (w < MAX_INT)
			{
				if (s.find(w) != s.end() || used[i][w]) break;
				s.insert(w);
			}
			int q = w;
			int p = 0;
			while (q != 0)
			{
				p += (q % i) * (q % i);
				q /= i;
			}
			w = p;
		}
		if (w != 1) 
		{
			for (set<int>::iterator it = s.begin(); it != s.end(); ++it)
			{
				used[i][*it] = 1;
			}
			return false;
		}
	}
	return true;
}

int main()
{
	freopen("precalc.txt", "w", stdout);
	for (int i = 1; i < 512; ++i)
	{
		int k = i;
		for (int j = 2; j <= 10; ++j)
		{
			tab[j] = (k % 2);
			k /= 2;
		}
		fill(used[0], used[11], 0);
		int x = 1;
		while (true)
			if (solve(++x)) break;
		printf("%d\n", x);
		fprintf(stderr, "%d, %d\n", i, x);
	}
}