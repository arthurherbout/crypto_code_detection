#include <stdio.h>
#include <stdlib.h>


int main(){
	int test,i;
	scanf("%d",&test);
	for (i = 0; i < test; ++i)
	{
		int n;
		int num[10],cnt=0;
		scanf("%d",&n);
		if (n==0)
		{
			printf("Case #%d: INSOMNIA\n",i+1 );
			continue;
		}
		int z;
		for (z = 0; z < 10; ++z)
		{
			num[z]=0;
		}

		int out,mult=1;
		while(cnt<10){
			
			//printArray(num,10);
			int iter=mult*n;
			out=mult*n;
			//printf("%d\n",iter);
			while(iter>0){
				int digit;
				digit=iter%10;
				if (num[digit]==0)
				{
					cnt+=1;
					num[digit]=1;
				}
				iter/=10;
			}
			mult++;
		
		}
		printf("Case #%d: %d\n",i+1,out );
	}
}
	