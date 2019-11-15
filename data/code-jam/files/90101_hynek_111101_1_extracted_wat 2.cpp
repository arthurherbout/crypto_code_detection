#include <stdio.h>
#include <stdlib.h>
#include <queue>
#define N 110
#define MOC 2000000000

using namespace std;

int pole[N*N];
int vysl[N*N];
int t,h,w;
int akt,neco;
int pocet;

int tece_do(int x)
{
	int poc=0,nejn=MOC;
	int sou[10];
	if(x / N > 0) //North
	{
		sou[poc]=x-N;
		poc++;
	}
	if(x % N > 0) //West
	{
		sou[poc]=x-1;
		poc++;
	}
	if( (x % N) + 1 < w) //East
	{
		sou[poc]=x+1;
		poc++;
	}
	if( (x / N) + 1 < h) //South
	{
		sou[poc]=x+N;
		poc++;
	}
	for(int k=0;k<poc;k++)
	{
		if(pole[sou[k]] < nejn) nejn=pole[sou[k]];
	}
	if(pole[x] <= nejn) return -1;
	for(int k=0;k<poc;k++)
	{
		if(pole[sou[k]] == nejn) return sou[k];
	}
	return -42;
}

int main()
{
	//fclose(stdin);
	//stdin=fopen("wat.in","r");
		
	scanf("%d",&t);
	for(int pripad=0;pripad<t;pripad++)
	{
		pocet=1;
		printf("Case #%d:\n",pripad+1);
		scanf("%d%d",&h,&w);
		for(int i=0;i<h;i++) for(int j=0;j<w;j++)
		{
			scanf("%d",pole+N*i+j);
			vysl[N*i+j]=0;
		}
		for(int i=0;i<h;i++) for(int j=0;j<w;j++)
		{
			akt=N*i+j;
			if(vysl[akt]) continue;
			while(akt>=0)
			{
				if(vysl[akt] && neco == 0 && vysl[akt] != neco)
				{
					neco=vysl[akt];
					akt=N*i+j;
					//printf("akt=%d neco=%d\n",akt,neco);
					continue;
				}
				vysl[akt]= ( neco ? neco : pocet );
				akt=tece_do(akt);
			}
			if(neco)
			{
				neco=0;
			}
			else
			{
				pocet++;
			}
		}
		for(int i=0;i<h;i++)
		{
			for(int j=0;j<(w-1);j++) printf("%c ",'a'-1+vysl[N*i+j]);
			printf("%c\n",'a'-1+vysl[N*i+w-1]);
		}
	}

	return 0;
}
