#include <cstdio>
#include <cstring>
#include <map>
#include <queue>

using namespace std;

map <int,int> M,v;
queue <int> Q;

int main(){
	int tt,cas,x,y,i,n;
	scanf("%d",&tt);
	for (cas=1;cas<=tt;cas++){
		scanf("%d",&n);
		M.clear();
		v.clear();
		int ans=0;
		while (!Q.empty()) Q.pop();
		for (i=1;i<=n;i++){
			scanf("%d%d",&x,&y);
			M[x]+=y;
			if (M[x]>1&&v[x]==0){
				Q.push(x);
				v[x]=true;
			}
		}
		while (!Q.empty()){
			x=Q.front();
			Q.pop();
			y=M[x]/2;
			M[x]-=y*2;
			M[x-1]+=y;
			M[x+1]+=y;
			ans+=y;
			if (M[x-1]>1&&!v[x-1]){
				v[x-1]=true;
				Q.push(x-1);
			}
			if (M[x+1]>1&&!v[x+1]){
				v[x+1]=true;
				Q.push(x+1);
			}
			v[x]=false;
		}
		printf("Case #%d: ",cas);
		printf("%d\n",ans);
	}
	return 0;
}
