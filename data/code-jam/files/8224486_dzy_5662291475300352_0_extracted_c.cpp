#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
typedef long long ll;
const int mo=1e9+7;
int pow(int a,int b,int c){int ret=1;for(;b;b>>=1,a=1LL*a*a%c)if(b&1)ret=1LL*ret*a%c;return ret;}


int n,a[1005],b[1005],c[1005];
int main(){
	freopen("C-small-1-attempt1.in","r",stdin);
	freopen("data.out","w",stdout);
	int tc,T=0;
	scanf("%d",&tc);
	while(tc--){
		scanf("%d",&n);
		for(int i=1;i<=n;i++) scanf("%d%d%d",&a[i],&b[i],&c[i]);
		int ans=0;
		if(n==2){
			if(a[1] < a[2]) swap(a[1], a[2]), swap(c[1], c[2]);
			double t1 = 1.0*c[1] * (360 - 1.0 * a[1]) / 360.0;
			double p = (a[2] + t1 * 360.0 / (1.0*c[2]));
			if(p<=360){
				double t2=1.0*c[2]*(360-1.0*a[2])/360.0;
				double q=(a[1]+t2*360.0/(1.0*c[1]));
				if(q>=360.0){
					ans=1;
				}
			}else if(p>=720.0){
				ans=1;
			}
		}
		printf("Case #%d: %d\n",++T,ans);
	}
	return 0;
}

