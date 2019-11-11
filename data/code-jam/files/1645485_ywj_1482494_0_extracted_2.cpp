#include<stdio.h>
#include<algorithm>
using namespace std;
//
//struct node{
//	int lev1,lev2;
//}le[1010];

//bool cmp(node a,node b){
//	if(a.lev1!=b.lev1){
//		return a.lev1<b.lev2;
//	}
//	return a.lev1>b.lev2;
//}

int le[1010][2],fle[1010];

int lev1(int n,int sta){
	int max=0,id=-1;
	for(int i=0;i<n;i++){
		if(fle[i]==0&&sta>=le[i][0]){
			if(le[i][1]>max){
				max=le[i][1];
				id=i;
			}
		}
	}
	return id;
}

int lev2(int n,int sta){
	for(int i=0;i<n;i++){
		if(fle[i]<2&&sta>=le[i][1]){
			return  i;
		}
	}
	return -1;
}

int pd(int n){
	for(int i=0;i<n;i++){
		if(fle[i]!=2)return -1;
	}
	return 1;
}

int main(){
	freopen("3.in","r",stdin);
	freopen("3.out","w",stdout);
	int T;
	scanf("%d",&T);
	for(int cas=1;cas<=T;cas++){
		int n;
		int count=0;
		scanf("%d",&n);
		for(int i=0;i<n;i++){
			scanf("%d%d",&le[i][0],&le[i][1]);
		}
		for(int i=0;i<n;i++){
			fle[i]=0;
		}
		int star=0;
		while(1){
			int id;
			while(1){
				id=lev2(n,star);
				if(id==-1)break;
				count++;
				if(fle[id]==0){
					star+=2;
				}
				else{
					star+=1;
				}
				fle[id]=2;
			}
			while(1){
				id=lev1(n,star);
				if(id==-1)break;
				star+=1;
				fle[id]=1;
				count++;
				while(1){
					id=lev2(n,star);
					if(id==-1)break;
					count++;
					if(fle[id]==0){
						star+=2;
					}
					else{
						star+=1;
					}
					fle[id]=2;
				}
			}
			if(id==-1)break;
		}
		printf("Case #%d: ",cas);
		if(pd(n)==-1){
			printf("Too Bad\n");
		}
		else{
			printf("%d\n",count);
		}

	}
	return 0;
}
