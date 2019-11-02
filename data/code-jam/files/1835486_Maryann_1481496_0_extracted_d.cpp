#include<stdio.h>
#include<string.h>
bool g[36][36],edge[36],ok[36];
int deg[36],ace[36],ans;
char s[1010];
int to(char x){
	if(x=='o')return 0;
	if(x=='i')return 1;
	if(x=='e')return 3;
	if(x=='a')return 4;
	if(x=='s')return 5;
	if(x=='t')return 7;
	if(x=='b')return 8;
	if(x=='g')return 9;
	return -1;
}
int ancestor(int x){return x-ace[x]?ace[x]=ancestor(ace[x]):x;}
int main(){
	int _;
	scanf("%d",&_);
	for(int t=1; t<=_; t++){
		scanf("%*d%s",s);
		memset(g,0,sizeof(g));
		int l=strlen(s);
		for(int i=1; i<l; i++){
			int A=s[i-1]-'a'+10,B=s[i]-'a'+10,A1=to(s[i-1]),B1=to(s[i]);
			g[A][B]=true;
			if(A1!=-1)g[A1][B]=true;
			if(B1!=-1)g[A][B1]=true;
			if(A1!=-1 && B1!=-1)g[A1][B1]=true;
		}
		memset(deg,0,sizeof(deg));
		memset(edge,0,sizeof(edge));
		for(int i=0; i<36; i++)ace[i]=i;
		ans=0;
		for(int i=0; i<36; i++)
			for(int j=0; j<36; j++)
				if(g[i][j]){
					ans++;
					deg[i]++;
					deg[j]--;
					edge[i]=edge[j]=true;
					ace[ancestor(i)]=ancestor(j);
				}
		memset(ok,0,sizeof(ok));
		for(int i=0; i<36; i++)if(deg[i]>0){
			ans+=deg[i];
			ok[ancestor(i)]=true;
		}
		for(int i=0; i<36; i++)
			if(ancestor(i)==i && edge[i] && !ok[i])
				ans++;
		printf("Case #%d: %d\n",t,ans);
	}
	return 0;
}
