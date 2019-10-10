#include<stdio.h>
#include<string.h>
bool G[110][110],b[200];
int a[110][30],link[200],n;
bool find(int i){
    for(int j=0;j<n;++j){
        if(G[i][j]&&!b[j]){
            b[j]=true;
            if(link[j]==-1||find(link[j])){
                link[j]=i;
                return true;
            }
        }
    }
    return false;
}


int main(){
	int t,k,i,j,x,y,sum;
	freopen("2.in","r",stdin);
	freopen("2.out","w",stdout);
	scanf("%d",&t);
	for(y=1;y<=t;++y){
		scanf("%d%d",&n,&k);
		for(i=0;i<n;++i)
			for(j=0;j<k;++j)
				scanf("%d",&a[i][j]);
		memset(G,false,sizeof(G));
		for(i=1;i<n;++i){
			for(j=0;j<i;++j){
				for(x=0;x<k;++x)
					if(a[i][x]<=a[j][x])break;
				if(x>=k)G[j][i]=true;
				else {
					for(x=0;x<k;++x)
						if(a[i][x]>=a[j][x])break;
					if(x>=k)G[i][j]=true;
				}
			}
		}
		memset(link,-1,sizeof(link));
        for(sum=0,i=0;i<n;++i){
            memset(b,false,sizeof(b));
            if(find(i))++sum;
        }
		printf("Case #%d: %d\n",y,n-sum);
	}
	return 0;
}


