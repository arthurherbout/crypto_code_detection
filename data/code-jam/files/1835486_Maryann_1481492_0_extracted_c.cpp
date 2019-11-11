#include<stdio.h>
#include<algorithm>
#define price first
#define time second
using namespace std;
int dp[2000001];
int n,ans;
int t;
pair<int,int> a[205];
bool update(int newd,int d,int q,int p){
	long long int w=q-p*(newd-d+0LL);
	if(w>=0){
		int ww=(int)w;
		if(dp[newd]<ww){
			dp[newd]=ww;
			ans=max(ans,newd);
		}
		return true;
	}else{
		ans=max(ans,d+q/p);
		return false;
	}
}
int main(){
	freopen("D:\\C-small-attempt3.in","r",stdin);
	freopen("D:\\C-small-attempt3.out","w",stdout);
	int _,m,money,fee;
	scanf("%d",&_);
	for(t=1; t<=_; t++){
		scanf("%d%d%d",&money,&fee,&m);
		for(int i=0; i<m; i++){
			scanf("%d%d",&a[i].price,&a[i].time);
			a[i].time++;
		}
		memset(dp,-1,sizeof(dp));
		sort(a,a+m);
		int prev=-1;
		n=0;
		for(int i=0; i<m; i++)
			if(a[i].time>prev){
				prev=a[i].time;
				a[n++]=a[i];
			}
		dp[0]=money;
		ans=0;
		fprintf(stderr,"%d %d %d\n",t,money,n);
		for(int u=0; u<=ans; u++)
			if(dp[u]!=-1){
				int qq=dp[u];
				if(qq>=fee){
					qq-=fee;
					int now=u;
					for(int i=0; i<n; i++){
						int T=i?a[i].time-a[i-1].time:a[i].time;
						if(!update(now+T,now,qq,a[i].price))break;
						now+=T;
						qq-=T*a[i].price;
					}
			}
		}
		printf("Case #%d: %d\n",t,ans);
	}
	return 0;
}