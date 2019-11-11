#include<stdio.h>
#include<string.h>
bool s[300];
int a[100];
int main(){
	freopen("1.in","r",stdin);
	freopen("1.out","w",stdout);
	char str[100];
	int i,j,n,m,ll,t,k,b;
	scanf("%d",&t);
	for(b=1;b<=t;++b){
		scanf("%s",str);
		memset(s,false,sizeof(s));
		for(i=0;str[i]!='\0';++i)
			s[str[i]]=true;
		ll=i;
		if(ll==1){printf("Case #%d: 1\n",b);continue;}
		for(i=j=0;i<256;++i)
			if(s[i])++j;
		m=j;
		if(m==1)m++;
		memset(a,-1,sizeof(a));
		a[0]=1;
		for(i=1;i<ll;++i){
			if(str[i]==str[0])a[i]=1;
		}
		for(j=0;j<ll;++j)
			if(a[j]==-1)break;
		if(ll>1)a[j]=0;
		for(i=j+1;i<ll;++i){
			if(str[i]==str[j])a[i]=0;
		}
		for(k=2;k<m;++k){
			for(i=0;i<ll;++i){
				if(a[i]==-1)break;
			}
			if(i>=ll)break;
			a[i]=k;
			for(j=i+1;j<ll;++j){
				if(str[j]==str[i])a[j]=k;
			}
		}
		long long ans=0;
		for(i=0;i<ll;++i)
			ans=ans*m+a[i];
		printf("Case #%d: %lld\n",b,ans);
	}
	return 0;
}

