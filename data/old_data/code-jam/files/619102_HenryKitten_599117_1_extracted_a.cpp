#include <cstdio>

int A[1000],B[1000];

int main(){
	int n;
	int ncas,cas;
	scanf("%d",&ncas);
	for(int cas=1;cas<=ncas;cas++){
		scanf("%d",&n);
		for(int i=0;i<n;i++) scanf("%d%d",&A[i],&B[i]);
		
		int nsect = 0;
		for(int i=0;i<n;i++){
			for(int j=i+1;j<n;j++){
				nsect += (((A[i]-A[j])*(B[i]-B[j]))<0);
			}
		}
		printf("Case #%d: %d\n",cas,nsect);
	}
	return 0;
}
