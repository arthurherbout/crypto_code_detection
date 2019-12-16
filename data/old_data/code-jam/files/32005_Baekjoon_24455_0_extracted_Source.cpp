#include <cstdio>
#include <memory.h>
int a[11][11];
int b[11][11];
int dx[]= {-1,0,0,1};
int dy[] = {0,-1,1,0};
int ans = 0;
int n,m,sx,sy;
void go(int x){
	if(a[sx][sy]==0)
		return;
	if(ans < x)
		ans=x;
	memset(b,0,sizeof(b)); 
	for(int i=1; i<=n; i++){
		for(int j=1; j<=m; j++){
			if(i==sx && j==sy)
				continue;
			int mm=0;
			for(int k=0; k<4; k++){
				if(i+dx[k]>=1 && i+dx[k]<=n && j+dy[k]>=1 && j+dy[k]<=m){
					if(mm<a[i+dx[k]][j+dy[k]])
						mm=a[i+dx[k]][j+dy[k]];
				}
			}
			if(mm!=0){
				for(int k=0; k<4; k++){
					if(i+dx[k]>=1 && i+dx[k]<=n && j+dy[k]>=1 && j+dy[k]<=m){
						if(mm==a[i+dx[k]][j+dy[k]]){
							b[i+dx[k]][j+dy[k]]+=  a[i][j];
							break;
						}
					}
				}
			}
		}
	}
	int t1[11][11];
	int t2[11][11];
	for(int k=0; k<4; k++){
		if(sx+dx[k]>=1 && sx+dx[k]<=n && sy+dy[k]>=1 && sy+dy[k]<=m){
			memcpy(t1,a,sizeof(a));
			memcpy(t2,b,sizeof(b));
			b[sx+dx[k]][sy+dy[k]] += a[sx][sy];
			if(a[sx+dx[k]][sy+dy[k]]==0)
				b[sx+dx[k]][sy+dy[k]]=0;
			bool ok = true;
			for(int i=1; i<=n; i++){
				for(int j=1; j<=m; j++){
					if(b[i][j]!=0)
						ok= false;
				}
			}
			if(ok){
				ans= -1;
				return;
			}
			for(int i=1; i<=n; i++)
				for(int j=1; j<=m; j++)
					a[i][j]-=b[i][j];
			for(int i=1; i<=n; i++){
				for(int j=1; j<=m; j++)
					if(a[i][j]<0)
						a[i][j] = 0;
			}
			go(x+1);
			if(ans==-1)
				return;
			memcpy(a,t1,sizeof(a));
			memcpy(b,t2,sizeof(b));
		}
	}
}
int main(){
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	int t;
	scanf("%d\n",&t);
	for(int tc=1; tc<=t; tc++){
		printf("Case #%d: ",tc);
		scanf("%d %d %d %d\n",&m,&n,&sy,&sx);
		for(int i=1; i<=n; i++){
			for(int j=1; j<=m; j++){
				scanf("%d ",&a[i][j]);
			}
		}
		ans = 0;
		go(1);
		if(ans==-1)
			printf("forever\n");
		else
			printf("%d day(s)\n",ans);
	}
	return 0;
}