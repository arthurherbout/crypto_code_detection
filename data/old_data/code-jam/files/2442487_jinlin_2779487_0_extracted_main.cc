#include <iostream>
#include <cstring>
#include <cstdio>

int a[4000], b[4000], f[4000], ans[40000], pre[40000];
int mp[4000][4000];

int main() {
	int T;
	scanf("%d", &T);
	for(int cas=1; cas<=T; ++cas) {
		int n;
		scanf("%d", &n);
		for(int i=0; i<n; ++i)
			scanf("%d", a+i);
		for(int i=0; i<n; ++i)
			scanf("%d", b+i);
		for(int i=0; i<n; ++i)
			f[i] = 0;
		memset(mp, 0, sizeof(mp));
		memset(pre,-1,sizeof(pre));
		for(int i=0; i<n; ++i) {
			if(pre[a[i]]!=-1) mp[i][pre[a[i]]] = 1;
			pre[a[i]] = i;
			if(pre[a[i]-1]!=-1) mp[pre[a[i]-1]][i] = 1;
		}
		memset(pre,-1,sizeof(pre));
		for(int i=n-1; i>=0; --i) {
			if(pre[b[i]]!=-1) mp[i][pre[b[i]]] = 1;
			pre[b[i]] = i;
			if(pre[b[i]-1]!=-1) mp[pre[b[i]-1]][i] = 1;
		}
		for(int i=0; i<n; ++i)
			for(int j=0; j<n; ++j)
				if(mp[i][j]) ++f[i];
		for(int i=0; i<n; ++i) {
			int k=0;
			for(int j=0; j<n; ++j)
				if(f[j]==0) k=j;
			f[k]=-1;
			for(int j=0; j<n; ++j) {
				if(f[j]<0) continue;
				if(mp[j][k]) --f[j];
			}
			ans[k] = n-i;
		}
		printf("Case #%d:", cas);
		for(int i=0; i<n; ++i)
			printf(" %d", ans[i]);
		printf("\n");
		for(int i=0; i<n; ++i) {
		}
	}
	return 0;
}
