#include<stdio.h>
int a[111],res[3];
int main(){
	int R,K;//N=3 M=5
	scanf("%*d%d%*d%*d%d",&R,&K);
	printf("Case #1:\n");
	while(R--){
		for(int i=0; i<K; i++)
			scanf("%d",&a[i]);
		int lc=0;
		while(1){
			bool good=false;
			for(int i=0; i<K; i++)
				if(a[i]%3==0){
					good=true;
					break;
				}
			if(good){
				res[lc++]=3;
				for(int i=0; i<K; i++)
					if(a[i]%3==0)a[i]/=3;
				putchar('3');
				continue;
			}

			for(int i=0; i<K; i++)
				if(a[i]%5==0){
					good=true;
					break;
				}
			if(good){
				res[lc++]=5;
				for(int i=0; i<K; i++)
					if(a[i]%5==0)a[i]/=5;
				putchar('5');
				continue;
			}
			break;
		}

		int large=1;
		for(int i=lc; i<3; i++)
			large*=4;

		bool flag=false;
		for(int i=0; i<K; i++)
			if(a[i]>=large/2){
				if(a[i]==large)
					for(int j=lc; j<3; j++)
						putchar('4');
				else{
					putchar('2');
					for(int j=lc+1; j<3; j++)
						putchar('4');					
				}
				flag=true;
				break;
			}

		if(!flag){
			putchar('2');
			for(int j=lc+1; j<3; j++)
				putchar('4');
		}
		puts("");
	}
	return 0;
}