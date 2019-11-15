#include<stdio.h>
#include<string.h>
#include<queue>
using namespace std;
struct node{
	int a[8],step;
};
queue<node>Q;
bool s[8][8][8][8][8][8][8][8];
char str[10][10];
int n;
bool pd(int a[]){
	int i,j,k;
	for(i=0;i<n;++i){
		for(j=a[i],k=i+1;k<n;++k){
			if(str[j][k]=='1')return false;
		}
	}
	return true;
}
int bfs(){
	int i,j;
	node tmp;
	for(i=0;i<n;++i)
		tmp.a[i]=i;
	for(;i<8;++i)
		tmp.a[i]=0;
	if(pd(tmp.a))return 0;
	s[tmp.a[0]][tmp.a[1]][tmp.a[2]][tmp.a[3]][tmp.a[4]][tmp.a[5]][tmp.a[6]][tmp.a[7]]=true;
	tmp.step=0;
	Q.push(tmp);
	node now;
	while(!Q.empty()){
		now=Q.front();
		Q.pop();
		for(i=0;i<n-1;++i){
			for(j=0;j<i;++j)
				tmp.a[j]=now.a[j];
			tmp.a[j++]=now.a[i+1];
			tmp.a[j++]=now.a[i];
			for(;j<n;++j)
				tmp.a[j]=now.a[j];
			for(;j<8;++j)
				tmp.a[j]=0;
			if(!s[tmp.a[0]][tmp.a[1]][tmp.a[2]][tmp.a[3]][tmp.a[4]][tmp.a[5]][tmp.a[6]][tmp.a[7]]){
				s[tmp.a[0]][tmp.a[1]][tmp.a[2]][tmp.a[3]][tmp.a[4]][tmp.a[5]][tmp.a[6]][tmp.a[7]]=true;
				if(pd(tmp.a))return now.step+1;
				tmp.step=now.step+1;
				Q.push(tmp);
			}
		}
	}
	return -1;
}
int main(){
	int t,i,j;
	freopen("A-small-attempt0.in","r",stdin);
	freopen("A-small-attempt0.out","w",stdout);
	scanf("%d",&t);
	for(j=1;j<=t;++j){
		scanf("%d",&n);
		for(i=0;i<n;++i)
			scanf("%s",str[i]);
		memset(s,false,sizeof(s));
		while(!Q.empty())Q.pop();
		printf("Case #%d: %d\n",j,bfs());
	}
	return 0;
}
