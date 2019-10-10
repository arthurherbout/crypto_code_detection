#include<stdio.h>
#include<string.h>
int n,m;
int ne[5000],v[5000];
int st[500];
int dist0[500],dist1[500];
int q[500],op,cl;
int dp[500][500];
bool f[500][500];

int ord[500][500],len[500];
void bfs(int x,int dist[500]){
	op=cl=0;
	q[op++]=x;
	for(int i=0; i<n; i++)
		dist[i]=-1;
	dist[x]=0;
	while(cl<op){
		x=q[cl++];
		for(int i=st[x]; i!=-1; i=ne[i]){
			int y=v[i];
			if(dist[y]==-1){
				dist[y]=dist[x]+1;
				q[op++]=y;
			}
		}
	}
}
int main(){
	int _,t;
	scanf("%d",&_);
	for(t=1; t<=_; t++){
		scanf("%d%d",&n,&m);
		memset(st,-1,sizeof(st));
		memset(f,0,sizeof(f));
		for(int i=0,e=0; i<m; i++){
			int x,y;
			scanf("%d%*c%d",&x,&y);
			f[x][y]=f[y][x]=true;
			v[e]=y;
			ne[e]=st[x];
			st[x]=e++;
			v[e]=x;
			ne[e]=st[y];
			st[y]=e++;
		}
		if(f[0][1]){
			int ans=0;
			for(int i=1; i<n; i++)
				if(f[0][i])
					ans++;
			printf("Case #%d: %d %d\n",t,0,ans);
			continue;
		}
		bfs(0,dist0);
		bfs(1,dist1);
		printf("Case #%d: %d ",t,dist0[1]-1);
		for(int i=0; i<n; i++)
			len[i]=0;
		for(int i=0; i<n; i++)
			if(dist0[i]+dist1[i]==dist0[1]){
				int x=dist0[i];
				ord[x][len[x]++]=i;
			}
		memset(dp,-1,sizeof(dp));
		for(int i=0; i<len[1]; i++){
			int x=ord[1][i];
			dp[0][x]=0;
			for(int j=1; j<n; j++)
				if(j!=x && (f[0][j]||f[x][j]))
					dp[0][x]++;
		}
		int ans=0;
		for(int i=1; i<dist0[1]; i++){
			for(int i1=0; i1<len[i-1]; i1++){
				int x=ord[i-1][i1];
				for(int i2=0; i2<len[i]; i2++){
					int y=ord[i][i2];
					if(dp[x][y]==-1)continue;
					if(i==dist0[1]-1){
						if(dp[x][y]>ans)ans=dp[x][y];
					}else{
					for(int i3=0; i3<len[i+1]; i3++){
						int z=ord[i+1][i3],now=dp[x][y]-1;
						for(int w=st[z]; w!=-1; w=ne[w]){
							int vert=v[w];
							if(!f[x][vert] && !f[y][vert] && vert!=x && vert!=y)now++;
						}
						if(now>dp[y][z])dp[y][z]=now;
					}
					}
				}
			}
		}
		printf("%d\n",ans);
	}
	return 0;
}
