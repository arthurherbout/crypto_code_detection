#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<algorithm>
#include<sstream>
#include<stdio.h>
#include<cstring>
#define fr(i,a,b) for(i=a;i<=b;++i)
using namespace std;
const int maxn=1000002;
int ca,ti,n,m,k,i,j,a,b,c,l;
double g[maxn],h[maxn],le[maxn],ri[maxn],ans;
double val(){
	int i;
	le[0]=1;
	fr(i,1,k)
		le[i]=le[i-1]*h[i];
	ri[k+1]=1;
	for(i=k;i>=1;i--)
		ri[i]=ri[i+1]*(1-h[i]);
	double tot=0;
	fr(i,0,k)
		tot+=le[i]*ri[i+1];
	return 1-tot;
}
int main(){
	freopen("b1.in","r",stdin);
	freopen("b1.out","w",stdout);
	cin>>ca;
	fr(ti,1,ca){
		cin>>n>>k;
		scanf("\n");
		m=0;
		fr(i,1,n){
			scanf("%d/%d %d\n",&a,&b,&c);
//			cout<<a<<" "<<b<<" "<<c<<endl;
			while(c--)
				g[++m]=double(a)/b;
		}
		sort(g+1,g+1+m);
		ans=1;
		fr(i,0,k){
			l=0;
			for(j=m;j>m-(k-i);j--)
				h[++l]=g[j];
			for(j=i;j>=1;j--)
				h[++l]=g[j];
			ans=min(ans,val());
		}
		cout<<"Case #"<<ti<<": ";
		printf("%.8lf\n",ans);
	}
}

