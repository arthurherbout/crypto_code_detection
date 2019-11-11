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

int n, m;
int T, tp = 0;
char str[10001];


int main( )
{

	scanf("%d", &T);
	while (T --)
	{
		scanf("%d %d", &n, &m);
		bool flag = false;
		for (int i = 1; i <= n; i ++)
		{
			int ans = 1;
			scanf("%s", str + 1);
			for (int j = 1; j <= m; j ++)
				ans &= (str[j] - '0');
			if (ans == 1) flag = true;
		}
		scanf("%s", str + 1);
		++ tp;
		printf("Case #%d: ", tp);
		if (flag == true) printf("IMPOSSIBLE\n");
		else
		{
			printf("0");
			for (int i = 1; i <= m - 1; i ++)
				printf("1");
			printf(" ");
			for (int i = 1; i <= m; i ++)
				printf("0?");
			printf("\n");
		}
	}
	return 0;
}
