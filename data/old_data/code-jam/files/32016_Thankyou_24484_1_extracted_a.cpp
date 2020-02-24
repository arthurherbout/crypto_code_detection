#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;
typedef long long llong;
const int MAXN = 1000;

llong x[MAXN], y[MAXN];
int n;
llong ans;

int main()
{
	//freopen("A-large.in", "r", stdin);
	//freopen("A-large.out", "w", stdout);
	
	int t;
	scanf("%d", &t);
	int kth = 1;
	while (t--)
	{
		scanf("%d", &n);
		for (int i=0; i<n; i++)
			scanf("%lld", &x[i]);
		for (int i=0; i<n; i++)
			scanf("%lld", &y[i]);
		sort(x, x+n);
		sort(y, y+n);
		reverse(y, y+n);	
		ans = 0;
		for (int i=0; i<n; i++)
			ans += x[i]*y[i];
		printf("Case #%d: %lld\n", kth++, ans);
	}	
	return 0;
}
