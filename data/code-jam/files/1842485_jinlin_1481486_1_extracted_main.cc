#include <algorithm>
#include <cstring>
#include <cstdio>

using namespace std;

int dp[20000];
int d[20000];
int l[20000];

int main() {
	int t, n;
	scanf("%d", &t);
	for(int cas=1; cas<=t; ++cas) {
		scanf("%d", &n);
		for(int i=0; i<n; ++i)
			scanf("%d%d", d+i, l+i);
		memset(dp, -1, sizeof(int)*(n+1));
		scanf("%d", d+n);
		l[n] = 1;
		dp[0] = d[0];
		for(int i=0; i<n; ++i) {
			if(dp[i]==-1) continue;
			for(int j=i+1; j<=n&&d[j]<=dp[i]+d[i]; ++j)
				dp[j] = max(dp[j], min(l[j], d[j]-d[i]));
		}
		printf("Case #%d: ", cas);
		if(dp[n]==1) printf("YES\n");
		else printf("NO\n");
	}
	return 0;
}
