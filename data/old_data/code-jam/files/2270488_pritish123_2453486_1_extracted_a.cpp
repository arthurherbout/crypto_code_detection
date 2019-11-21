#include<cstdio>

int main()
{
	int a,b,c,t,d;
	scanf("%d",&t);
	char set[5][5];
	char won; 
	for(a=1;a<=t;a++)
	{
		won='N';
		for(b=0;b<4;b++)
		{
			scanf("%s",set[b]);
		}
		for(b=0;b<4;b++)
		{
			d=0;
			for(c=0;c<4;c++)
			{
				if(set[b][c]=='X'||set[b][c]=='T')
					d++;
			}
			if(d==4)
			{
				won='X';
				break;
			}
		}
		for(b=0;b<4;b++)
		{
			d=0;
			for(c=0;c<4;c++)
			{
				if(set[c][b]=='X'||set[c][b]=='T')
					d++;
			}
			if(d==4)
			{
				won='X';
				break;
			}
		}
		
		
		
		for(b=0;b<4;b++)
		{
			d=0;
			for(c=0;c<4;c++)
			{
				if(set[c][b]=='O'||set[c][b]=='T')
					d++;
			}
			if(d==4)
			{
				won='O';
				break;
			}
		}
		for(b=0;b<4;b++)
		{
			d=0;
			for(c=0;c<4;c++)
			{
				if(set[b][c]=='O'||set[b][c]=='T')
					d++;
			}
			if(d==4)
			{
				won='O';
				break;
			}
		}
		d=0;
		for(c=0;c<4;c++)
			{
				if(set[c][c]=='O'||set[c][c]=='T')
					d++;
			}
			if(d==4)
			{
				won='O';
			}
		d=0;
		for(c=0;c<4;c++)
			{
				if(set[c][c]=='X'||set[c][c]=='T')
					d++;
			}
			if(d==4)
			{
				won='X';
			}
			
		d=0;
		for(c=0;c<4;c++)
			{
				if(set[3-c][c]=='O'||set[3-c][c]=='T')
					d++;
			}
			if(d==4)
			{
				won='O';
			}
		d=0;
		for(c=0;c<4;c++)
			{
				if(set[3-c][c]=='X'||set[3-c][c]=='T')
					d++;
			}
			if(d==4)
			{
				won='X';
			}
			
		
		
	
		if(won=='N')
		{
			for(b=0;b<4;b++)
			{
			d=0;
			for(c=0;c<4;c++)
			{
				if(set[b][c]=='.')
					d++;
			}
			}
			if(d>0)
			{
				printf("Case #%d: Game has not completed\n",a);
			}
			else
			{
				printf("Case #%d: Draw\n",a);
			}
			
		}
		else
		{
			printf("Case #%d: %c won\n",a,won);
		}
		//printf("\nwon = %c\n",won);
	}
	
	
	
	return 0;
}
