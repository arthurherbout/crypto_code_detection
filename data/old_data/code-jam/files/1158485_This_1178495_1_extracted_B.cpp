/*
 * 2011-06-11  Martin  <Martin@Martin-desktop>

 * 
 */
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

template <class T> inline T checkmin(T &a, T b)
{
	return (a < b) ? a : a = b;
}

#define pb push_back

int N;
int Cnt[10005];
vector <int> now, next;

inline int Solve()
{
	memset(Cnt, 0, sizeof(Cnt));
	scanf("%d", &N);
	for (int i = 1; i <= N; ++ i)
	{
		int x;
		scanf("%d", &x);
		++ Cnt[x];
	}
	if (N == 0)
		return 0;
	int ans = 0x7fffffff;
	int last = - 1;
	now.clear();
	next.clear();
	for (int i = 1; i <= 10000; ++ i)
		if (Cnt[i] > 0)
		{
			if (last + 1 == i)
			{
				sort(now.begin(), now.end());
				next.clear();
				int t = Cnt[i];
				for (int j = 0; j < (int) now.size(); ++ j)
				{
					if (t == 0)
					{
						checkmin(ans, now[j]);
						break;
					}
					next.pb(now[j] + 1);
					-- t;
				}
				while (t > 0)
					next.pb(1), -- t;
				now = next;
			}
			else
			{
				sort(now.begin(), now.end());
				if (!now.empty())
					checkmin(ans, now[0]);
				now.clear();
				for (int j = 1; j <= Cnt[i]; ++ j)
					now.pb(1);
			}
			last = i;
		}
	sort(now.begin(), now.end());
	checkmin(ans, now[0]);
	return ans;
}

int main()
{
	int TestCase;
	scanf("%d", &TestCase);
	for (int i = 1; i <= TestCase; ++ i)
		printf("Case #%d: %d\n", i, Solve());
	return 0;
}
