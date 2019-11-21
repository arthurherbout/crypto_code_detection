#include<stdio.h>
#include<algorithm>
using namespace std;
#define inf 1013741823
int n,m,d;
int a[52][52][52][2][2];
char s[99][99];
int dp(int x,int l,int r,bool lf,bool rf)
{
	if(x>=n-1)return 0;
	if(x==n-2)
	{
		if(lf||rf)return 0;
		if(r-l>1)return 1;
		return inf;
	}
	if(a[x][l][r][lf][rf]!=-1)
		return a[x][l][r][lf][rf];
	a[x][l][r][lf][rf]=inf;
	if(lf)//fall from the left, i.e column l-1
	{
		int x2;
		for(x2=x+1; x2<n && s[x2][l-1]=='.'; x2++); x2--;
		if(x2-x<=d)
		{
			int l2=l-1,r2=l;
			for(;l2>0 && s[x2][l2-1]=='.' && s[x2+1][l2-1]=='#'; l2--);
			for(;r2<m && s[x2][r2]=='.' && s[x2+1][r2]=='#'; r2++);
			a[x][l][r][lf][rf]=min(a[x][l][r][lf][rf],dp(x2,l2,r2,l2>0 && s[x2][l2-1]=='.', r2<m-1 && s[x2][r2]=='.'));
		}
	}
	if(rf)//fall from the right, i.e column r
	{
		int x2;
		for(x2=x+1; x2<n && s[x2][r]=='.'; x2++); x2--;
		if(x2-x<=d)
		{
			int l2=r,r2=r+1;
			for(;l2>0 && s[x2][l2-1]=='.' && s[x2+1][l2-1]=='#'; l2--);
			for(;r2<m && s[x2][r2]=='.' && s[x2+1][r2]=='#'; r2++);
			a[x][l][r][lf][rf]=min(a[x][l][r][lf][rf],dp(x2,l2,r2,l2>0 && s[x2][l2-1]=='.', r2<m-1 && s[x2][r2]=='.'));
		}
	}
	if(r-l>1)
	{
		for(int i=l; i<r; i++)
			if(s[x+2][i]=='.')
			{
				int x2;
				for(x2=x+2; x2<n && s[x2][i]=='.'; x2++); x2--;
				if(x2-x<=d)
				{
					int l2=i,r2=i+1;
					for(;l2>0 && s[x2][l2-1]=='.' && s[x2+1][l2-1]=='#'; l2--);
					for(;r2<m && s[x2][r2]=='.' && s[x2+1][r2]=='#'; r2++);
					a[x][l][r][lf][rf]=min(a[x][l][r][lf][rf],dp(x2,l2,r2,l2>0 && s[x2][l2-1]=='.', r2<m-1 && s[x2][r2]=='.')+1);
				}
			}
		for(int i=l; i<r; i++)
			if(s[x+2][i]=='#')
				for(int j=i; j<r; j++)
					if(s[x+2][j]=='#' && (i>l || j<r-1))
					{
						int l2=i,r2=j+1,x2=x+1;
						for(;l2>0 && s[x2][l2-1]=='.' && s[x2+1][l2-1]=='#'; l2--);
						for(;r2<m && s[x2][r2]=='.' && s[x2+1][r2]=='#'; r2++);
						a[x][l][r][lf][rf]=min(a[x][l][r][lf][rf],dp(x2,l2,r2,l2>0 && s[x2][l2-1]=='.', r2<m-1 && s[x2][r2]=='.')+j-i+1);
					}
					else
						break;
	}
	return a[x][l][r][lf][rf];
}
int main()
{
	int t,_,c;
	scanf("%d",&_);
	for(t=1; t<=_; t++)
	{
		scanf("%d%d%d",&n,&m,&d);
		memset(a,-1,sizeof(a));
		for(int i=0; i<n; i++)
			scanf("%s",s[i]);
		printf("Case #%d: ",t);
		for(c=0; c<m && s[0][c]=='.' && s[1][c]=='#'; c++);
		int ans=dp(0,0,c,false,s[0][c]=='.');
		if(ans>=inf)
			puts("No");
		else
			printf("Yes %d\n",ans);
	}
	return 0;
}