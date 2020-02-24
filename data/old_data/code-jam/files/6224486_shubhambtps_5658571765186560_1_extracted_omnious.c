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
		else if((X==5)&&((R*C)%5==0)&&(R>=5||C>=5)&&(R>3&&C>3))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==6)&&((R*C)%6==0)&&(R>=6||C>=6)&&(R>4&&C>4))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==7)&&((R*C)%7==0)&&(R>=7||C>=7)&&(R>5&&C>5))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==8)&&((R*C)%8==0)&&(R>=8||C>=8)&&(R>6&&C>6))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==9)&&((R*C)%9==0)&&(R>=9||C>=9)&&(R>7&&C>7))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==10)&&((R*C)%10==0)&&(R>=10||C>=10)&&(R>8&&C>8))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==11)&&((R*C)%11==0)&&(R>=11||C>=11)&&(R>9&&C>9))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==12)&&((R*C)%12==0)&&(R>=12||C>=12)&&(R>10&&C>10))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==13)&&((R*C)%13==0)&&(R>=13||C>=13)&&(R>11&&C>11))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==14)&&((R*C)%14==0)&&(R>=14||C>=14)&&(R>12&&C>12))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==15)&&((R*C)%15==0)&&(R>=15||C>=15)&&(R>13&&C>13))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==16)&&((R*C)%16==0)&&(R>=16||C>=16)&&(R>14&&C>14))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==17)&&((R*C)%17==0)&&(R>=17||C>=17)&&(R>15&&C>15))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==18)&&((R*C)%18==0)&&(R>=18||C>=18)&&(R>16&&C>16))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==19)&&((R*C)%19==0)&&(R>=19||C>=19)&&(R>17&&C>17))
		{
			printf("Case #%d: GABRIEL\n",count);
		}
		else if((X==20)&&((R*C)%20==0)&&(R>=20||C>=20)&&(R>18&&C>18))
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
