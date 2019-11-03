#include<stdio.h>
int n,m;
long long int a[600][600];
long long int t[600][600],hti[600][600];
long long int vti[600][600];
void init(){
	for(int i=1; i<=n; i++)
		for(int j=1; j<=m; j++)
			hti[i][j]=a[i][j]*j;
	for(int i=1; i<=n; i++)
		for(int j=1; j<=m; j++)
			hti[i][j]+=hti[i-1][j];
	for(int i=1; i<=n; i++)
		for(int j=1; j<=m; j++)
			hti[i][j]+=hti[i][j-1];
	for(int i=1; i<=n; i++)
		for(int j=1; j<=m; j++)
			vti[i][j]=a[i][j]*i;
	for(int i=1; i<=n; i++)
		for(int j=1; j<=m; j++)
			vti[i][j]+=vti[i-1][j];
	for(int i=1; i<=n; i++)
		for(int j=1; j<=m; j++)
			vti[i][j]+=vti[i][j-1];
	for(int i=1; i<=n; i++)
		for(int j=1; j<=m; j++)
			t[i][j]=a[i][j];
	for(int i=1; i<=n; i++)
		for(int j=1; j<=m; j++)
			t[i][j]+=t[i-1][j];
	for(int i=1; i<=n; i++)
		for(int j=1; j<=m; j++)
			t[i][j]+=t[i][j-1];
}
long long int calc(long long int a[600][600],int ax,int ay,int bx,int by){
	ax--;
	ay--;
	return a[ax][ay]+a[bx][by]-a[ax][by]-a[bx][ay];
}
bool check1(int x,int y,int z){
	long long int w=calc(hti,x-z,y-z,x+z,y+z)-calc(t,x-z,y-z,x+z,y+z)*y;
	w-=a[x-z][y-z]*(-z);
	w-=a[x+z][y-z]*(-z);
	w-=a[x-z][y+z]*z;
	w-=a[x+z][y+z]*z;
	if(w!=0)return false;
	w=calc(vti,x-z,y-z,x+z,y+z)-calc(t,x-z,y-z,x+z,y+z)*x;
	w-=a[x-z][y-z]*(-z);
	w-=a[x+z][y-z]*z;
	w-=a[x-z][y+z]*(-z);
	w-=a[x+z][y+z]*z;
	if(w!=0)return false;
	return true;
}
bool check2(int x,int y,int z){
	long long int w=calc(hti,x-z+1,y-z+1,x+z,y+z)*2-calc(t,x-z+1,y-z+1,x+z,y+z)*(2*y+1);
	w-=a[x-z+1][y-z+1]*(1-2*z);
	w-=a[x+z][y-z+1]*(1-2*z);
	w-=a[x-z+1][y+z]*(2*z-1);
	w-=a[x+z][y+z]*(2*z-1);
	if(w!=0)return false;
	w=calc(vti,x-z+1,y-z+1,x+z,y+z)*2-calc(t,x-z+1,y-z+1,x+z,y+z)*(2*x+1);
	w-=a[x-z+1][y-z+1]*(1-2*z);
	w-=a[x+z][y-z+1]*(2*z-1);
	w-=a[x-z+1][y+z]*(1-2*z);
	w-=a[x+z][y+z]*(2*z-1);
	if(w!=0)return false;
	return true;
}
int main(){
	int _,t,x,y,z,ans;
	char s[999];
	scanf("%d",&_);
	for(t=1; t<=_; t++){
		scanf("%d%d%*d",&n,&m);
		for(int i=1; i<=n; i++){
			scanf("%s",s);
			for(int j=1; j<=m; j++)
				a[i][j]=s[j-1]-'0';
		}
		init();
		ans=0;
		for(int i=1; i<=n; i++)
			for(int j=1; j<=m; j++)
				for(int k=3; i+k<=n+1 && j+k<=m+1; k++)
					if(k%2){
						x=i+k/2,y=j+k/2,z=k/2;
						if(check1(x,y,z)){
//							printf("%d %d %d\n",i,j,k);
							if(k>ans)ans=k;
						}
					}else{
						x=i+k/2-1,y=j+k/2-1,z=k/2;
						if(check2(x,y,z)){
//							printf("%d %d %d\n",i,j,k);
							if(k>ans)ans=k;
						}
					}
		printf("Case #%d: ",t);
		if(ans==0)puts("IMPOSSIBLE");else
			printf("%d\n",ans);
	}
	return 0;
}
