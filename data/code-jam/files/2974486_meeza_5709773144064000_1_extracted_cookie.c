#include<stdio.h>
int main(){
	int t;
	scanf("%d",&t);
	double results[t];
	for(int i = 0; i < t; i++){
		float c,f,x;
		scanf("%f",&c);
		scanf("%f",&f);
		scanf("%f",&x);
		double calc_time, min, temp1, temp2;
		
		// for n = 0
		calc_time = x/2;
		min = calc_time;
		int n = 1;
		temp1 = 0;
		while(1){
			temp2 = 2+(n*f);
			//printf("temp2 : %f\n",temp2);
			temp1 = temp1 + (c/(2+(n-1)*f));			
			calc_time = temp1 + (x/temp2);
			//printf("calc_time : %f\n",calc_time);
			//printf("min : %f\n",min);
			if(calc_time > min)
				break;
			else
				min = calc_time;
			n++;
		}
		//printf("value of n at %dth case: %d\n",i, n);
		results[i] = min;		
	}
	for(int i = 0; i < t; i++){
		printf("Case #%d: %f\n",i+1,results[i]);
	}
	return 0;
}
