#include<stdio.h>
#include<string.h>

const int maxn = 7;
const int dir[4][2]={ -1, 0,   0, -1,  0, 1,  1, 0};

int a[maxn][maxn];
int c, r, sc, sr;
int ans;

void dfs(int t, int a[][maxn])
{
	if(ans==-1) return;

	int b[maxn][maxn];
	memcpy(b, a, sizeof(b));
	
	int i,j,k,d,i1,j1;
	
	for(i=1;i<=r;i++)for(j=1;j<=c;j++)if(!(i==sr&&j==sc))
	{
		k = 0;
		for(d=1;d<4;d++)
		{
			if(a[i+dir[d][0]][j+dir[d][1]] > a[i+dir[k][0]][j+dir[k][1]]) k = d;
		}
		b[i+dir[k][0]][j+dir[k][1]] -= a[i][j];
		if(b[i+dir[k][0]][j+dir[k][1]]<0) b[i+dir[k][0]][j+dir[k][1]] = 0;
	}

	if(b[sr][sc]==0) {
		if(t>ans) ans = t;
		return;
	}
	
	i = sr; j = sc;
	int f;
	for(f=a[i][j]; f>=0; f--){
	for(k=0;k<4;k++)
	{
		int temp = b[i+dir[k][0]][j+dir[k][1]];
		b[i+dir[k][0]][j+dir[k][1]] -= f;
		if(b[i+dir[k][0]][j+dir[k][1]]<0) b[i+dir[k][0]][j+dir[k][1]] = 0;

		d = 1;
		for(i1=1;i1<=r && d;i1++)for(j1=1;j1<=c && d;j1++)if(b[i1][j1]!=a[i1][j1]) d = 0;
		if(d) { ans = -1; return; }
		dfs(t+1, b);
		
		b[i+dir[k][0]][j+dir[k][1]] = temp;
	}
	}
	//memcpy(a, b, sizeof(b));
}

int main() {
	int i,j,k;
	int cs,step;
	scanf("%d",&cs);
	for(step=1;step<=cs;step++)
	{
		printf("Case #%d: ", step);
		scanf("%d%d%d%d",&c,&r,&sc,&sr);
		memset(a, 0, sizeof(a));
		for(i=1;i<=r;i++)for(j=1;j<=c;j++)
		{
			scanf("%d",&a[i][j]);
		}
		
		ans = 0;
		dfs(1, a);
		if(ans==-1)printf("forever\n"); else printf("%d day(s)\n", ans);
	}
	return 0;
}
