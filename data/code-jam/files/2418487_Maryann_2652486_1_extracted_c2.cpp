#include<stdio.h>
#include<string.h>
long long int a[22];

int N;
int R,K;
int o[22],o2[22];
int nex[9]={-1,-1,3,4,6,-1,8,-1,1};
int e2[9]={0,0,1,0,2,0,1,0,3};
int e3[9]={0,0,0,1,0,0,1,0,0};
double s[9][38][13];
int c[22][22];
int a2[22],a3[22];
double res;

void dfs(int x,int tot){
	if(x==1){
		bool flag=true;
		for(int i=0; i<K; i++)
			if(s[1][a2[i]][a3[i]]==0){
				flag=false;
				break;
			}
		if(flag){
			double now=1;
			for(int i=0; i<K; i++)
				now*=s[1][a2[i]][a3[i]];
			if(now>res){
				res=now;
				memcpy(o,o2,sizeof(o));
			}
		}
		return;
	}
	int down=0,up=N-tot;
	if(x==8)down=up;
	for(int num=down; num<=up; num++){
		o2[x]=num;
		memset(s[nex[x]],0,sizeof(s[nex[x]]));
		for(int j=0; j<=num; j++){
			for(int t1=0; t1<=tot*3; t1++)
				for(int t2=0; t2<=tot; t2++)
					s[nex[x]][e2[x]*j+t1][e3[x]*j+t2]+=s[x][t1][t2]*c[num][j];
		}
		dfs(nex[x],tot+num);
	}
}

int main(){

	for(int i=0; i<=20; i++)
		for(int j=0; j<=i; j++)
			c[i][j]=j==0 || j==i?1:c[i-1][j]+c[i-1][j-1];

	scanf("%*d%d%*d%*d%d",&R,&K);
	printf("Case #1:\n");
	while(R--){
		for(int i=0; i<K; i++)
			scanf("%I64d",&a[i]);
		int lc=0;
		while(1){
			bool good=false;
			for(int i=0; i<K; i++)
				if(a[i]%7==0){
					good=true;
					break;
				}
			if(good){
				lc++;
				for(int i=0; i<K; i++)
					if(a[i]%7==0)a[i]/=7;
				putchar('7');
				continue;
			}

			for(int i=0; i<K; i++)
				if(a[i]%5==0){
					good=true;
					break;
				}
			if(good){
				lc++;
				for(int i=0; i<K; i++)
					if(a[i]%5==0)a[i]/=5;
				putchar('5');
				continue;
			}
			break;
		}

		for(int i=0; i<K; i++){
			a2[i]=a3[i]=0;
			while(a[i]%2==0)a[i]/=2,a2[i]++;
			while(a[i]%3==0)a[i]/=3,a3[i]++;
		}

		N=12-lc;
		res=0;
		s[2][0][0]=1;
		dfs(2,0);
		for(int i=2; i<=8; i++)
			for(int j=0; j<o[i]; j++)
				putchar(i+'0');
		puts("");
		fprintf(stderr,"%lf %d\n",res,R);
	}
	return 0;
}