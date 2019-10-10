#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

char s[2000];
int mp[2000][2000];
int sx[2000][2000];
int sy[2000][2000];
int sw[2000][2000];

int main() {
	int T;
	scanf("%d", &T);
	for(int cas=1; cas<=T; ++cas) {
		int n, m, d;
		scanf("%d%d%d", &n, &m, &d);
		for(int i=1; i<=n; ++i) {
			scanf("%s", s);
			for(int j=1; j<=m; ++j)
				mp[i][j] = s[j-1]-'0'+1;
		}
		memset(sx, 0, sizeof(sx));
		for(int i=1; i<=n; ++i)
			for(int j=1; j<=m; ++j)
				sx[i][j] = mp[i][j]*i+sx[i-1][j]+sx[i][j-1]-sx[i-1][j-1];
		memset(sy, 0, sizeof(sy));
		for(int i=1; i<=n; ++i)
			for(int j=1; j<=m; ++j)
				sy[i][j] = mp[i][j]*j+sy[i-1][j]+sy[i][j-1]-sy[i-1][j-1];
		memset(sw, 0, sizeof(sw));
		for(int i=1; i<=n; ++i)
			for(int j=1; j<=m; ++j)
				sw[i][j] = mp[i][j]+sw[i-1][j]+sw[i][j-1]-sw[i-1][j-1];
		int ans = 0;
		for(int k=3; k<=n&&k<=m; ++k) {
			for(int i=0; i+k<=n; ++i) {
				for(int j=0; j+k<=m; ++j) {
					int cw = sw[i+k][j+k]-sw[i][j+k]-sw[i+k][j]+sw[i][j];
					cw -= mp[i+1][j+1]+mp[i+1][j+k]+mp[i+k][j+1]+mp[i+k][j+k];
					int cx = sx[i+k][j+k]-sx[i][j+k]-sx[i+k][j]+sx[i][j];
					cx -= mp[i+1][j+1]*(1+i)+mp[i+1][j+k]*(1+i)+mp[i+k][j+1]*(i+k)+mp[i+k][j+k]*(i+k);
					cx *= 2;
					int cy = sy[i+k][j+k]-sy[i][j+k]-sy[i+k][j]+sy[i][j];
					cy -= mp[i+1][j+1]*(1+j)+mp[i+1][j+k]*(j+k)+mp[i+k][j+1]*(1+j)+mp[i+k][j+k]*(j+k);
					cy *= 2;
					//printf("%d-%d\n", cx, cy);
					if(cx%cw) continue;
					if(cy%cw) continue;
					cx /= cw;
					cy /= cw;
					//printf("%d:%d:%d-%d:%d:%d\n", i, cx, i+k, j, cy, j+k);
					if(cx!=i*2+k+1) continue;
					if(cy!=j*2+k+1) continue;
					ans = k;
				}
			}
		}
		printf("Case #%d: ", cas, ans);
		if(ans<3) printf("IMPOSSIBLE\n");
		else printf("%d\n", ans);
	}
}
