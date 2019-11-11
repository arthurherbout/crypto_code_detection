#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <set>
#include <map>
#include <vector>

#define mp make_pair
#define pb push_back

using namespace std;

typedef long long ll;

const int inf = 0x3f3f3f3f;

char str[1000001];
char stk[1000001];
int tp = 0;

int main( )
{
	int T;
	scanf("%d", &T);
	while (T --)
	{
		scanf("%s", str);
		int len = (int )strlen(str), a = 0, b = 0, ans = 0, top = 0;
		for (int i = 0; i < len; i ++)
		{
			stk[++ top] = str[i];
			while (top >= 2 && stk[top] == stk[top - 1])
				ans += 10, top -= 2;
		}
		++ tp;
		printf("Case #%d: %d\n", tp, ans + top / 2 * 5);
	}
	return 0;
}
