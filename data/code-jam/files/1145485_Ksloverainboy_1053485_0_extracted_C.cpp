#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>
const int MaxN=100;

using namespace std;
int head[MaxN],c[MaxN],s[MaxN],t[MaxN],T,n,tot,N,M,Ans,S,Lef;
void work(int Limit){
	S=0;
	for (int i=0;i<N;i++)
	head[i]=i;
	tot=N;
	int now=N;
	Lef=1;
	while (now<=Limit){
//for (int i=0;i<tot;i++)
//cout << head[i] << " ";
//cout << "]]]]]" << S << endl;
		if (Lef==0){
			Ans=max(Ans,S);
			return;
		}
		Lef--;
		if (tot==0) continue;
		int flag=0;
		for (int i=0;i<tot && !flag;i++)
		if (t[head[i]]){
			Lef+=t[head[i]];
			flag=1;
			S+=s[head[i]];
			for (int j=1;j<=c[head[i]] && now<Limit;j++){
				head[tot++]=now;
				now++;
			}
			tot--;
			for (int j=i;j<tot;j++)
			head[j]=head[j+1];
		}
		if (flag) continue;
		int nowc=-1,nows=-1,Now;
		if (now<Limit){
			for (int i=0;i<tot;i++){
				int x=head[i];
				if (c[x]>nowc || (c[x]==nowc && s[x]>nows)){
					Now=i;
					nowc=c[x];
					nows=s[x];
				}
			}
			if (nowc==1){
				S+=nows;
				Lef+=t[head[Now]];
				for (int j=1;j<=1 && now<Limit;j++){
					head[tot++]=now;
					now++;
				}
				tot--;
				for (int j=Now;j<tot;j++)
				head[j]=head[j+1];
			}
		}else{
			for (int i=0;i<tot;i++){
				int x=head[i];
				if (s[x]>nows){
					Now=i;
					nowc=c[x];
					nows=s[x];
				}
			}
			S+=nows;
			Lef+=t[head[Now]];
			for (int j=1;j<=nowc && now<Limit;j++){
				head[tot++]=now;
				now++;
			}
			tot--;
			for (int j=Now;j<tot;j++)
			head[j]=head[j+1];
		}
	}
//cout << S << endl;
}
int main(){
	freopen("C.in","r",stdin);
	freopen("C.out","w",stdout);
	scanf("%d", &T);
	for (int tt=1;tt<=T;tt++){
		printf("Case #%d: ", tt);
		scanf("%d", &N);
		n=0;
		for (int i=0;i<N;i++){
			scanf("%d %d %d", &c[n], &s[n], &t[n]);
			n++;
		}
		scanf("%d", &M);
		for (int i=0;i<M;i++){
			scanf("%d %d %d", &c[n], &s[n], &t[n]);
			n++;
		}
		Ans=-1;
		for (int i=N;i<=n;i++)
		work(i);
		printf("%d\n", Ans);
	}
}
