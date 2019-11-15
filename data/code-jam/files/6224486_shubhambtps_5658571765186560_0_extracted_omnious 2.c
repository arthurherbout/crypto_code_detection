#include<stdio.h>
int main()
{
	int T,count=0;
	int X,R,C;
	scanf("%d",&T);
	while(T--)
	{
		count++;
		scanf("%d%d%d",&X,&R,&C);
		if(X==1)
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if(((R*C)%2==0) && X==2 &&(R>=2 || C>=2))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==3)&&((R*C)%3==0)&&(R>=3||C>=3)&&(R>1&&C>1))
		{
			printf("Case #%d: GABRIEL\n",count);	
		}
		else if((X==4)&&((R*C)%4==0)&&(R>=4||C>=4)&&(R>2&&C>2))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else
		{
			printf("Case #%d: RICHARD\n",count);
		}
	}
	return 0;
}
