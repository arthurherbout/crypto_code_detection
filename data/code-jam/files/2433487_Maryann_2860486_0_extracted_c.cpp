#include<stdio.h>
#include<algorithm>
#define London 2
#define Mon 1
using namespace std;
int n,m,q;
int d[55][55],d2[55][55];
//d: if take longest path, SP
//d2: if take shortest path, SP
int u[55],v[55],s[55],l[55],id[55];

int usedTime[55];

int main(){
	int _,t;
	scanf("%d",&_);
	for(t=1; t<=_; t++){
		scanf("%d%d%d",&n,&m,&q);
		for(int i=1; i<=n; i++)
			for(int j=1; j<=n; j++){
				d[i][j]=i==j?0:1000000000;
				d2[i][j]=i==j?0:1000000000;
			}
		for(int i=1; i<=m; i++){
			scanf("%d%d%d%d",&u[i],&v[i],&s[i],&l[i]);
			d[u[i]][v[i]]=min(d[u[i]][v[i]],l[i]);
			d2[u[i]][v[i]]=min(d2[u[i]][v[i]],s[i]);
		}
		for(int k=1; k<=n; k++)
		for(int i=1; i<=n; i++)
			for(int j=1; j<=n; j++)
				d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
		for(int k=1; k<=n; k++)
		for(int i=1; i<=n; i++)
			for(int j=1; j<=n; j++)
				d2[i][j]=min(d2[i][j],d2[i][k]+d2[k][j]);

		usedTime[0]=0;
		int o=-1;
		for(int i=1; i<=q; i++){
			scanf("%d",&id[i]);
			if(o!=-1)continue;
			int S=u[id[i]],T=v[id[i]],L=s[id[i]];
			usedTime[i]=usedTime[i-1]+L;

			bool bad=false;

			for(int j=1; j<=i; j++){
				if(bad)break;
				for(int k=j; k<=i; k++)
					if(d[u[id[j]]][u[id[k]]] + usedTime[i]-usedTime[k-1] + d2[T][London] > d[u[id[j]]][London]){
						bad=true;
						break;
					}
			}

			for(int j=1; j<=i; j++){
				if(bad)break;
				for(int k=j; k<=i; k++)
					if(d[u[id[j]]][u[id[k]]] + usedTime[i]-usedTime[k-1] > d[u[id[j]]][T]){
						bad=true;
						break;
					}
			}

			if(bad){
				if(o==-1)
					o=id[i];
			}


			/*
			bool bad=true;
			for(int j=1; j<=n; j++){
				if(d[S][London]<d2[S][j]+d2[j][London])continue;
				if(d[S][j]>=L+d2[T][j]){
					bad=false;
					break;
				}
			}
			if(bad){
				if(o==-1)o=id;
			}*/
		}

		printf("Case #%d: ",t);
		if(o==-1)puts("Looks Good To Me");else
			printf("%d\n",o);
	}
	return 0;
}