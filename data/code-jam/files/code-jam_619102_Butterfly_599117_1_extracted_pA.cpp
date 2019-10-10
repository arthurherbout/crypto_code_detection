#include<cstdio>
#include<algorithm>
struct jiz{
	int A,B;
	bool operator<(const jiz &cmp)const{
		return A<cmp.A;
	}
}J[1000];
int T,N,ans;
int main(){
	freopen("pA.in","r",stdin);
	freopen("pA.out","w",stdout);
	scanf("%d",&T);
	for(int i=0;i<T;i++){
		ans=0;
		scanf("%d",&N);
		for(int j=0;j<N;j++)
			scanf("%d%d",&J[j].A,&J[j].B);
		std::sort(J,J+N);
		for(int j=0;j<N;j++)
		    for(int k=j+1;k<N;k++)
				if(J[j].B>J[k].B)ans++;
		printf("Case #%d: %d\n",i+1,ans);
	}
	
}
