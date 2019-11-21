#include<cstdio>

using namespace std;


int gcd(int a, int b){
	if(a<b) return gcd(b,a);
	

	if(b==0) return a;
	
	return gcd(b,a%b);
	
}


int mmc(int a, int b){
	return a*b/gcd(a,b);
}

int main(){
	int T, k;
	int n[10000];
	int a, b, ans;
	int N, L, H;
	scanf("%d",&T);
	k=1;
	while(T--){
		scanf("%d%d%d",&N,&L,&H);
		for(int i=0;i<N;i++) scanf("%d",&n[i]);
		
		for(ans=L;ans<=H;ans++){
			int ok=1;
			for(int i=0;i<N;i++)
				if(n[i]%ans!=0 && ans%n[i]!=0){
					ok=0;
					break;
				}
			if(ok) break;
		}
		
		if(ans>=L && ans<=H){
			printf("Case #%d: %d\n",k,ans);
		}else{
			printf("Case #%d: NO\n",k);
		}
		
		
		k++;
	}
	return 0;
}
