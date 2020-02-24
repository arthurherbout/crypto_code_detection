#include <stdio.h>
#include <algorithm>

using namespace std;

#define iset(i,a) ((a)&(1<<(i)))

int pd[4][4][66000];
char tab[4][4];
int di[]={0,1,0,-1, -1,-1,1,1},dj[]={1,0,-1,0,-1,1,-1,1};
int n,m;

int solve(int i, int j,int mask)
{
	int k;
	int nm;
	int ni,nj;
	int ret=0;
	int tmp;
	if(pd[i][j][mask]>=0)
		return pd[i][j][mask];
	nm=mask|(1<<(i*4+j));
	for(k=0;k<8 && !ret ;++k)
	{
		ni=i+di[k];
		nj=j+dj[k];
		if(ni<n && ni>=0 && nj<m && nj>=0 && tab[ni][nj]=='.' && !iset(ni*4+nj,mask) )
		{
			tmp=solve(ni,nj,nm);
			if(!tmp)
				ret=1;
		}
	}
	return (pd[i][j][mask]=ret);
}

int main()
{
	int t,tcnt;
	int ist,jst;
	int i,j,k;
	scanf("%d",&t);
	for(tcnt=1;tcnt<=t;++tcnt)
	{
		scanf("%d %d",&n,&m);
		for(i=0;i<n;++i)
			for(j=0;j<m;++j)
			{
				scanf(" %c",&tab[i][j]);
				if(tab[i][j]=='K')
				{
					ist=i;
					jst=j;
				}
			}
		for(i=0;i<4;++i)
			for(j=0;j<4;++j)
				for(k=0;k<=1<<16;++k)
					pd[i][j][k]=-1;
		printf("Case #%d: %c\n",tcnt,solve(ist,jst,0)?'A':'B');
	}
	return 0;
}




