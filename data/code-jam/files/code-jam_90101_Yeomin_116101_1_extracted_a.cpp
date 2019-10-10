#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

int l, d, n;
vector<string> str;
bool c[500][26];

int solve()
{
	int res = 0;
	for (int i = 0; i < d; ++i)
	{
		const char *s = str[i].c_str();
		int j;
		for (j = 0; j < l; ++j)
		{
			if (!c[j][*s - 'a']) break;
			s++;
		}
		if (j == l) ++res;
	}

	return res;
}

int main()
{
	scanf("%d%d%d", &l, &d, &n);
	for (int i = 0; i < d; ++i)
	{
		char st[1000];
		scanf("%s", st);
		str.push_back(st);
	}
	for (int i = 0; i < n; ++i)
	{
		fill(c[0], c[500], false);
		char st[1000];
		scanf("%s", st);
		char *s = st;
		int l = 0;
		while (*s != 0)
		{
			if (*s == '(')
			{
				s++;
				while (*s !=')')
				{
					c[l][*s - 'a'] = true;
					s++;
				}
				++l;
			}
			else if (*s >= 'a' && *s <= 'z')
			{
				c[l++][*s - 'a'] = true;
			}
			s++;
		}
		printf("Case #%d: %d\n", i + 1, solve());
	}
}