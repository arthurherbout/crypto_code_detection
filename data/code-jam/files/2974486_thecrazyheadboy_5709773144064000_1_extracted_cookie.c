#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int t, oi, flag;
	double C, F, X, cur, temp, total_time;
	scanf("%d",&t);
	FILE *fp;
    fp=fopen("/home/mridul/CFiles/gji/output2.txt", "w");
	for(oi=1; oi <=t; oi++)
	{
		scanf("%lf %lf %lf",&C,&F,&X);
		cur=2, total_time=0, flag=1;
		while(flag)
		{
			temp = C/cur; 
			//if((int)(C-last)%(int)cur != 0)
			//	temp+=1;
			if(temp + X/(cur+F) < X/cur)
			{
				cur+=F;
				total_time+=temp;
			}
			else
			{
				flag = 0;
				total_time += X/cur;
			}
			//printf("C=%lf F=%lf X=%lf cur=%lf temp=%lf total_time=%lf flag=%d\n\n",C,F,X,cur,temp,total_time,flag);
		}
		fprintf(fp,"Case #%d: %.7lf\n",oi ,total_time);
	}
	fclose(fp);
	return(0);
}