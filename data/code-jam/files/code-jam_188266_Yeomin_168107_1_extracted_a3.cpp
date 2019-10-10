#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <string>
#include <cassert>
#include <set>

using namespace std;

#define MAX_INT 100000

int pretable[512];
int tab[] = {0, 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512};

int main()
{
	int t;
	int cases = 0;
	char buf[1000];
	FILE *pre = fopen("precalc.txt", "r");
	for (int i = 0; i < 512; ++i)
	{
		fscanf(pre, "%d", &pretable[i]);
	}
	fclose(pre);

	scanf("%d", &t);
	gets(buf);
	while (t--)
	{
		gets(buf);
		int q = 0;
		char *s = buf;
		while (*s != 0)
		{
			if (*s >= '0' && *s <= '9')
			{
				q += tab[atoi(s)];
				while (*s >= '0' && *s <= '9') s++;
				continue;
			}
			s++;
		}

		printf("Case #%d: %d\n", ++cases, pretable[q - 1]);
	}
}