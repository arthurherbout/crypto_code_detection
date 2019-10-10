#include <stdio.h>
#include <algorithm>

using namespace std;

int main(){
	int ncas;
	scanf("%d",&ncas);
	for(int t=1;t<=ncas;t++){
		long long n;
		int pd,pg;
		scanf("%lld%d%d",&n,&pd,&pg);
		if(pg == 100 && pd != pg || pg == 0 && pd != pg){
			printf("Case #%d: Broken\n",t);
			continue;
		}

		bool broken = true;
		bool used[101] = {0};
		for(int i=1;((long long)i)<=n;i++){
			int modulo = i*pd % 100;
			if(used[modulo]) break;
			used[modulo] = true;
			if(modulo == 0){
				broken = false;
				break;
			} 
		}	
		if(broken) printf("Case #%d: Broken\n",t);
		else printf("Case #%d: Possible\n",t);
	}
	return 0;
}
