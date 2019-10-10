#include <cstdio>

int main()
{
	int d, t[4][4], n, c[16];
	scanf("%d", &d);
	for(int i=1; i<=d; i++)
	{
		for(int j=0; j<16; j++)
			c[j]=0;
		printf("Case #%d: ", i);
		scanf("%d", &n);
		for(int j=0; j<4; j++)
			for(int k=0; k<4; k++)
			{
				scanf("%d", &t[j][k]);
				if(j+1 == n)
					c[t[j][k]-1]++;
			}
		scanf("%d", &n);
		for(int j=0; j<4; j++)
			for(int k=0; k<4; k++)
			{
				scanf("%d", &t[j][k]);
				if(j+1 == n)
					c[t[j][k]-1]++;
			}
		int res=-1, chk=0;
		for(int j=0; j<16; j++)
		{
			if(c[j]==2)
			{
				chk++;
				res = j+1;
			}
		}
		if(chk==1)
			printf("%d", res);
		else if(chk==0)
			printf("Volunteer cheated!");
		else
			printf("Bad magician!");
		if(i<d)
			puts("");
	}
	return 0;
}
