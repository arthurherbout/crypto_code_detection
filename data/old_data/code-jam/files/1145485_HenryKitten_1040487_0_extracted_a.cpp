#include <stdio.h>
#include <algorithm>

using namespace std;

int main(){
	int ncas;
	scanf("%d",&ncas);
	for(int t=1;t<=ncas;t++){
		int n,pd,pg;
		scanf("%d%d%d",&n,&pd,&pg);
		if(pg == 100 && pd != pg || pg == 0 && pd != pg){
			printf("Case #%d: Broken\n",t);
			continue;
		}

		bool broken = true;
		for(int i=1;i<=n;i++){
			if(i*pd  % 100 == 0){
				broken = false;
				break;
			} 
		}	
		if(broken) printf("Case #%d: Broken\n",t);
		else printf("Case #%d: Possible\n",t);
	}
	return 0;
}
