#include<stdio.h>

int main()
{
		int tcase,i,j,tcase2;
		double ct,ft,xt,sum=0,fig1,fig2, rate = 2.0;
		scanf("%d", &tcase);
		tcase2 = tcase;
		while(tcase--)
		{	
			sum = 0;
			rate = 2.0;
			//Input
			scanf("%lf", &ct);
			scanf("%lf", &ft);
			scanf("%lf", &xt);
			
			//calculations 
		    for(i = 0; ;i++)
			{
				fig1 = xt/ rate;
				fig2 = ct / rate + xt / (rate + ft);
				if(fig1 <= fig2)
				{
					sum += fig1; 
					printf("Case #%d: %lf\n", tcase2-tcase, sum);
					break;
				}
				sum += ct/rate;
				rate += ft;
			}
		}
}				
