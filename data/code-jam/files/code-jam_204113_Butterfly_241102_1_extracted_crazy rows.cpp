/*
Coder: Butterfly21
Task: Crazy Rows
Source: Google Code Jam 2009 Round2 Problem A
*/
#include<cstdio>
int T,N,pos[100],cnt;
char line[100];
int main(){
	freopen("A-large.in","r",stdin);
	freopen("A-large.out","w",stdout);
	scanf("%d",&T);
	for(int i=0;i<T;i++){
		cnt=0;
		scanf("%d\n",&N);
		for(int j=0;j<N;j++){
			gets(line);
			int k=N-1;
			while(k>=0&&line[k]!='1')k--;
			pos[j]=k+1;
		}
		for(int j=0;j<N;j++){
			if(pos[j]-1>j){
				int k=j+1;
				while(pos[k]-1>j)k++;
				for(;k>j;k--,cnt++)pos[k-1]^=pos[k]^=pos[k-1]^=pos[k];
			}
		}
		printf("Case #%d: %d\n",i+1,cnt);
	}
}
