#include <stdio.h>
#include <memory.h>
#define MOD 1000000007
long long suu[71][35*71+1];
long long dy[71][71], dd[71][71], ft[71][71];
long long N, B;
int sss[100], x;
int main(){
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	int T, t, i, j, k, l, n, ti;
	scanf("%d",&T);
	for(t=1;t<=T;t++){
		printf("Case #%d: ", t);
		scanf("%lld %lld",&N,&B);
		n = (int)(B*(B+1))/2;
		memset(suu,0,sizeof(suu));
		suu[0][0] = 1;
		for(i=0;i<=B;i++){
			for(j=0;j<=i;j++){
				ft[i][j] = 1;
				for(k=j+1;k<=i;k++){
					ft[i][j] = (ft[i][j] * k) % MOD;
				}
			}
		}
		for(i=1;i<B;i++){
			for(k=B;k>=1;k--){
				for(j=n;j>=i;j--){
					suu[k][j] = (suu[k][j] + suu[k-1][j-i])%MOD;
				}	
			}
		}
		x = 0;
		while(N>0){
			sss[x++] = N % B;
			N = N / B;
		}
		memset(dy, 0, sizeof(dy));
		dy[0][0] = 1;
		int next;
		int u;
		for(u=x-1;u>=0;u--){
			memset(dd, 0, sizeof(dd));
			for(i=0;i<=B;i++){
				for(j=0;j<=B;j++){
					if(dy[i][j] != 0){
						next = j * B + sss[u];
						for(k=i;k<=B;k++){
							for(l=0;l<=B && l<=next;l++){
								if(k == 2 && l == 0){
									k=k;
								}
								dd[k][l] = (dd[k][l] + ((dy[i][j] * suu[k][next - l])%MOD) * ft[k][k-i])%MOD;
								if(i > 0){
									dd[k][l] = (dd[k][l] + (((dy[i][j] * i)%MOD * suu[k-1][next - l])%MOD) * ft[k-1][k-i])%MOD;
								}
							}
						}
					}
				}
			}
			for(i=0;i<=B;i++)for(j=0;j<=B;j++) dy[i][j] = dd[i][j];
		}
		long long sol = 0;
		for(i=0;i<=B;i++){
			sol = (sol + dy[i][0])%MOD;
		}
		printf("%I64d\n",sol);
	}
	return 0;
}