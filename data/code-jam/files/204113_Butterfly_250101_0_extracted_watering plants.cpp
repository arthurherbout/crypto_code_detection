/*
Coder: Butterfly21
Task: Watering Plants
Source: Google Code Jam 2009 Round2 Problem D
*/
#include<cstdio>
#include<cmath>
struct plant{
	int X,Y,R;
}P[100];
int C,N;
double ans;
double min(double a,double b){return a<b?a:b;}
double max(double a,double b){return a>b?a:b;}
double dis(int a,int b){return sqrt(double((P[a].X-P[b].X)*(P[a].X-P[b].X)+(P[a].Y-P[b].Y)*(P[a].Y-P[b].Y)));}
int main(){
	freopen("D-small.in","r",stdin);
	freopen("D-small.out","w",stdout);
	scanf("%d",&C);
	for(int i=0;i<C;i++){
		ans=1e9;
		scanf("%d",&N);
		for(int j=0;j<N;j++)
			scanf("%d%d%d",&P[j].X,&P[j].Y,&P[j].R);
		if(N==1)ans=P[0].R*2.0;
		else if(N==2)ans=max(P[0].R,P[1].R)*2.0;
		else if(N==3){
			ans=min(ans,max(P[2].R,dis(0,1)+P[0].R+P[1].R));
			ans=min(ans,max(P[1].R,dis(2,0)+P[2].R+P[0].R));
			ans=min(ans,max(P[0].R,dis(2,1)+P[2].R+P[1].R));
		}
		printf("Case #%d: %.6lf\n",i+1,ans/2.0);
	}
}
