/*
Coder: Butterfly21
Task: Collecting Cards
Source: Google Code Jam 2009 Round1A Problem A
*/
#include<cstdio>
double C[41][41]={},CA[41][3000];
double P[41][41][41][41]={};
int T,M,N;
void comb(){
	for(int i=0;i<41;i++)C[i][0]=1.0;
	for(int i=1;i<41;i++)
	    for(int j=1;j<=i;j++)C[i][j]=C[i-1][j-1]+C[i-1][j];
}
inline double posb(int all,int pack,int alr,int get){
	if(P[all][pack][alr][get]>-0.5)return P[all][pack][alr][get];
	return P[all][pack][alr][get]=(C[alr][pack-get]*C[all-alr][get])/C[all][pack];
}
double calc(int rem,int dep){
	if(CA[rem][dep]>-0.5)return CA[rem][dep];
	if(dep>=3000)return 0.0;
	double tmp=0.0,sum=0.0;
	for(int i=0;i<=N&&i<=rem;i++){
		tmp=posb(M,N,M-rem,i);
		//printf("%d %d %d %d:%.5lf\n",M,N,M-rem,i,tmp);
		if(i==rem)sum+=tmp*double(dep);
		else sum+=tmp*calc(rem-i,dep+1);
	}
	return CA[rem][dep]=sum;
}
int main(){
	freopen("C-large.in","r",stdin);
	freopen("C-large.out","w",stdout);
	for(int i=0;i<41;i++)
        for(int j=0;j<41;j++)
            for(int k=0;k<41;k++)
                for(int l=0;l<41;l++)P[i][j][k][l]=-1.0;

	comb();
	//printf("%lf\n",C[40][20]);
	scanf("%d",&T);
	for(int i=1;i<=T;i++){
		scanf("%d%d",&M,&N);
			for(int j=0;j<41;j++)
	    for(int k=0;k<3000;k++)CA[j][k]=-1.0;
		if(M==N)printf("Case #%d: %.7lf\n",i,1.0);
		else printf("Case #%d: %.7lf\n",i,calc(M-N,2));
	}
}
