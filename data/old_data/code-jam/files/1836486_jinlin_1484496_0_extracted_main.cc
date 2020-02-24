#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <ctime>

using namespace std;

int MOD;

long long s[1000], pre[10000], nxt[10000];
char dp[500][10000];

inline
int reg(long long x) {
	return (x%MOD+MOD)%MOD;
}

inline
void mymin(int i, int x, long long y, const char & c) {
	if(abs(nxt[x])>abs(y)) {
		nxt[x] = y;
		dp[i][x] = c;
	}
}

const long long INF = 1000*1000000000000LL;

int main() {
	int T;
	srand(time(NULL));
	scanf("%d", &T);
	for(int cas=1; cas<=T; ++cas) {
		int n;
		scanf("%d", &n);
		for(int i=0; i<n; ++i)
			scanf("%lld", s+i);
		while(true) {
			random_shuffle(s, s+n);
			int k = rand()%10000+1;
			MOD = k;
			for(int i=0; i<k; ++i)
				nxt[i] = INF;
			memset(dp, 0, sizeof(dp));
			for(int i=0; i<n; ++i) {
				memcpy(pre, nxt, sizeof(pre));
				mymin(i, reg(s[i]), s[i], '>');
				mymin(i, reg(-s[i]), -s[i], '<');
				for(int j=0; j<k; ++j) {
					mymin(i, reg(j+s[i]), pre[j]+s[i], '+');
					mymin(i, reg(j-s[i]), pre[j]-s[i], '-');
				}
			}
			if(nxt[0]==0LL||nxt[0]>=INF/2)
				break;
		}
		printf("Case #%d:\n", cas);
		if(nxt[0]>=INF/2) {
			printf("Impossible\n");
			continue;
		}
		vector<long long> s1, s2;
		int cur = 0;
		for(int i=n-1; i>=0; --i) {
			if(dp[i][cur]=='+'||dp[i][cur]=='>') {
				s1.push_back(s[i]);
				if(dp[i][cur]=='>') break;
				cur = reg(cur-s[i]);
			} else if(dp[i][cur]=='-'||dp[i][cur]=='<') {
				s2.push_back(s[i]);
				if(dp[i][cur]=='<') break;
				cur = reg(cur+s[i]);
			}
		}
		for(int i=0; i<s1.size(); ++i) {
			if(i) printf(" ");
			printf("%lld", s1[i]);
		}
		printf("\n");
		for(int i=0; i<s2.size(); ++i) {
			if(i) printf(" ");
			printf("%lld", s2[i]);
		}
		printf("\n");
	}
}
