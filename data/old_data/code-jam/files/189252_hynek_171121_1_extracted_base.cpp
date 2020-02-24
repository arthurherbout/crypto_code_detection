#include <stdio.h>
#include <stdlib.h>

using namespace std;

int t,c;
int prevod[100];
int abeceda[300];
int base,delka;
unsigned long long int vysl;

int main()
{
	scanf("%d",&t);
	getc(stdin);
	for(int i=1;i<=t;i++)
	{
		for(int j=0;j<300;j++) abeceda[j]=-1;
		delka=0;
		base=0;
		while(1)
		{
			c=getc(stdin);
			if( ( c < '0' || c > '9' ) && ( c < 'a' || c > 'z' ) ) break;
			if( abeceda[c] == -1 )
			{
				abeceda[c] = base;
				base++;
			}
			prevod[delka] = abeceda[c];
			delka++;
		}
		if(base < 2) base = 2;
		for(int j=0;j<100;j++)
		{
			if( prevod[j] == 1) { prevod[j]=0; continue; }
			if( prevod[j] == 0) { prevod[j]=1; continue; }
		}
		printf("Case #%d: ",i);
		//for(int j=0;j<delka;j++) printf("%d",prevod[j]);
		//printf(" : base=%d\n",base);
		vysl=0;
		for(int j=0;j<delka;j++)
		{
			vysl *= base; 
			vysl += prevod[j];
		}
		printf("%llu\n",vysl);
	}
	return 0;
}
