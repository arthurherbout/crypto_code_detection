#include <stdio.h>
#include <map>
#include <string>
using namespace std;

int N;
int hi[16];
int h[16];

void run(int fall){
	printf("Case #%d:", fall);
	scanf("%d", &N);
	for(int i=0;i<N-1;i++){
		scanf("%d", &hi[i]);
		hi[i]--;
	}
	int fel;
	int guess=0;
	do{
		guess++;
		/*printf("guess %d\n", guess++);

		for(int i=0;i<N;i++){
			printf(" %d", h[i]);
		}
		printf("\n");*/
		fel=0;
		for(int i=0;i<N;i++){
			h[i] = rand() % 1000000000;
		}
		for(int i=0;i<N-1;i++){
			double maxlut=-1000000000;
			int maxluti=-1;
			for(int j=i+1;j<N;j++){
				double lut = ((double)h[j]-h[i])/(j-i);
				if(lut>maxlut){
					maxlut = lut;
					maxluti = j;
				}
			}
			if(hi[i] != maxluti){
				/*if(i>0)
				printf("fel %d ville ha %d men var %d\n", i, hi[i], maxluti);*/
				fel = 1;
				break;
			}
		}
		if(guess > 50000000){
			printf(" Impossible\n");
			return;
		}
	}while(fel);

	for(int i=0;i<N;i++){
		printf(" %d", h[i]);
	}
	printf("\n");
	//printf("guess %d\n", guess);
	/*for(int i=0;i<N-1;i++){
		double maxlut=-1000000000;
		int maxluti=-1;
		for(int j=i+1;j<N;j++){
			double lut = ((double)h[j]-h[i])/(j-i);
			if(lut>maxlut){
				maxlut = lut;
				maxluti = j;
			}
		}
		printf("%d %lf\n", maxluti, maxlut);
		if(hi[i] != maxluti){			
			printf("fel %d ville ha %d men var %d\n", i, hi[i], maxluti);
			fel = 1;
			break;
		}
	}*/
}


int main(){
	int T;
	scanf("%d", &T);
	for(int i=0;i<T;i++){
		run(i+1);
	}	
}