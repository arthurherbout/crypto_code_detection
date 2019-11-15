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

int stk[101];
int a[101][101];
char str[101][101];
int N, ans = 0x3f3f3f3f;
int n;
bool flag = false;
bool vis[1001];

void dfs2(int step)
{
	if (step == N + 1)
	{
		flag = true;
		return ;
	}
	int cur = stk[step];
	for (int i = 1; i <= n; i ++)
		if (a[i][cur])
		{
			if (vis[i]) continue;
			vis[i] = true;
			dfs2(step + 1);
			vis[i] = false;
		}
}

bool check()
{
	for (int i = 1; i <= n; i ++)
	{
		N = 0;
		vis[i] = true;
		for (int j = 1; j <= n; j ++)
			if (a[i][j]) stk[++ N] = j;
		flag = false;
		dfs2(1);
		vis[i] = false;
		if (flag) return false;
	}
	return true;
}

void dfs(int x, int y, int sum)
{
	if (x == n + 1)
	{
		if (check()) ans = min(ans, sum);
		return ;
	}
	if (a[x][y] == 0)
	{
		a[x][y] = 1;
		int xx = x, yy = y + 1;
		if (yy == n + 1) xx ++, yy = 1;
		dfs(xx, yy, sum + 1);
		a[x][y] = 0;
	}
	int xx = x, yy = y + 1;
	if (yy == n + 1) xx ++, yy = 1;
	dfs(xx, yy, sum);
}

int main( )
{
	int tp = 0, T;
//	freopen("input.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);
	scanf("%d", &T);
	while (T --)
	{
		ans = 0x3f3f3f3f;
		scanf("%d", &n);
		for (int i = 1; i <= n; i ++)
			scanf("%s", str[i] + 1);
		for (int i = 1; i <= n; i ++)
			for (int j = 1; j <= n; j ++)
				a[i][j] = str[i][j] - '0';
		
		dfs(1, 1, 0);
		++ tp;
		printf("Case #%d: %d\n", tp, ans);
	}
	return 0;
}
