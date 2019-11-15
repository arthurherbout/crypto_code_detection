#include<stdio.h>

int len[10010],dis[10010];
int mudi[10010];

//int serach(int id,int n,int swlen,int tmpdis){
//	int retid=-1;
//	int max=0;
//	for(;id<n;id++){
//		if(dis[id]-tmpdis>swlen)break;
//		int tmpmin=dis[id]-tmpdis<len[id]?dis[id]-tmpdis:len[id];
//		if(dis[id]+tmpmin>max){
//			max=dis[id]+tmpmin;
//			retid=id;
//		}
//	}
//	return retid;
//}

int main(){
	freopen("1e.in","r",stdin);
	freopen("1e.out","w",stdout);
	int T;
	scanf("%d",&T);
	for(int x=1;x<=T;x++){
		int n;
		scanf("%d",&n);
		for(int i=0;i<n;i++){
			scanf("%d %d",&dis[i],&len[i]);
		}
		int D;
		scanf("%d",&D);
		bool flag=false;
		if(len[0]<dis[0]){
			printf("Case #%d: NO\n",x);
			continue;
		}
		mudi[0]=dis[0];
		for(int i=1;i<n;i++){
			mudi[i]=-1;
		}
		for(int i=0;i<n;i++){
			for(int j=i+1;j<n;j++){
				if(dis[j]>dis[i]+mudi[i])break;
				int swlen=dis[j]-dis[i]<len[j]?dis[j]-dis[i]:len[j];
				if(swlen>mudi[j])mudi[j]=swlen;
			}
		}
		for(int i=0;i<n;i++){
			if(mudi[i]!=-1&&dis[i]+mudi[i]>=D){
				flag=true;
				break;
			}
		}
		if(flag){
			printf("Case #%d: YES\n",x);
		}
		else{
			printf("Case #%d: NO\n",x);
		}
	}
	return 0;
}


//int tmpdis=dis[0],swlen=dis[0];
//		int id=1;
//		while(1){
//			if(tmpdis+swlen>=D){
//				flag=true;
//				break;
//			}
//			if(id>=n)break;
//			int tmpid=serach(id,n,swlen,tmpdis);
//			if(tmpid==-1)break;
//			swlen=dis[tmpid]-tmpdis<len[tmpid]?dis[tmpid]-tmpdis:len[tmpid];
//			tmpdis=dis[tmpid];
//			id=tmpid+1;
//		}
//		if(flag){
//			printf("Case #%d: YES\n",x);
//		}
//		else{
//			printf("Case #%d: NO\n",x);
//		}