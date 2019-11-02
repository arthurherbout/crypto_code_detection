#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <functional>

using namespace std;

int t;
long long int n;
char c;
int delka,zac,x;
int pole[30];


int main()
{
	scanf("%d",&t);
	scanf("%c",&c);
	for(int i=1;i<=t;i++)
	{
		delka=0;
		pole[0]='0';
		while(1)
		{
			scanf("%c",&c);
			if( c < '0' || c > '9' ) break;
			delka++;
			pole[delka] = c;
		}
		for(int j=delka-1;j>=0;j--)
		{
			if(j == 0)
			{
				for(int i=delka;i>0;i--)
					if(pole[i] != '0')
					{
						x=i;
						break;
					}
				swap(pole[0],pole[x]);
				zac=1;
				break;
			}
			if( pole[j] < pole[j+1] )
			{
				zac=j+1;
				x=25;
				for(int i=delka;i>0;i--)
					if(pole[i] > pole[j] && (x==25 || pole[i] < pole[x]))
					{
						x=i;
						break;
					}
				swap(pole[j],pole[x]);
				break;
			}
		}
		sort(pole+zac, pole+delka+1, less<int>() );
		printf("Case #%d: ",i);
		if( pole[0] != '0' ) printf("%c",pole[0]);
		for(int j=1;j<=delka;j++) printf("%c",pole[j]);
		printf("\n");
	}
	
	return 0;
}
