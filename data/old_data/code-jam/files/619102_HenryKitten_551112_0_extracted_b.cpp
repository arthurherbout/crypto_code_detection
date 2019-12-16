#include <cstdio>

int main(){
	int n;
	int ncas,cas;
	scanf("%d",&ncas);
	for(int cas=1;cas<=ncas;cas++){
		int nt = 0;
		int ct = 0;
		int a,b,c;
		scanf("%d%d%d",&a,&b,&c);
		int base = a;
		for(ct=0;;ct++){
			if(base*c >= b) break;
			base *= c;
		}

		while(ct != 0){
			nt++;
			ct >>= 1;
		}
		
		printf("Case #%d: %d\n",cas,nt);
	}
	return 0;
}
