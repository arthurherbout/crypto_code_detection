#include<stdio.h>
#include<string.h>
#include<iostream>
#include<set>
#include<algorithm>
using namespace std;
struct node
{
	bool d[61][61];
};
char s[61][61];
bool d[61][61];
int m,n;
bool operator <(const node &a,const node &b)
{
	int i,j;
	for (i=0;i<m;i++)
		for (j=0;j<n;j++)
			if (a.d[i][j]!=b.d[i][j])
				return (int)a.d[i][j]<(int)b.d[i][j];
	return 0;
}
set<node> h;
void dfs2(int x,int y)
{
	if (d[x][y]==1)
		return;
	d[x][y]=1;
	if (x>0&&s[x-1][y]!='#')
		dfs2(x-1,y);
	if (y>0&&s[x][y-1]!='#')
		dfs2(x,y-1);
	if (y<n-1&&s[x][y+1]!='#')
		dfs2(x,y+1);
	return;
}
bool dfs(int &x,int &y,node r)
{
	int i,j,t;
	t=0;
	for (i=0;i<m;i++)
	{
		for (j=0;j<n;j++)
		{
			if (r.d[i][j]==1&&d[i][j]==0)
				return 0;
			if (r.d[i][j]==1&&(i!=x||j!=y))
			{
				t=1;
				break;
			}
		}
		if (t==1)
			break;
	}
	if (t==0)
		return 1;
	if (h.find(r)!=h.end())
		return 0;
	h.insert(r);
	node nextr;
	memset(nextr.d,0,sizeof(nextr.d));
	for (i=0;i<m;i++)
	{
		for (j=0;j<n;j++)
		{
			if (r.d[i][j]==1)
			{
				if (i+1<m&&s[i+1][j]!='#')
					nextr.d[i+1][j]=1;
				else
					nextr.d[i][j]=1;
			}
		}
	}
	if (dfs(x,y,nextr))
		return 1;
	memset(nextr.d,0,sizeof(nextr.d));
	for (i=0;i<m;i++)
	{
		for (j=0;j<n;j++)
		{
			if (r.d[i][j]==1)
			{
				if (j+1<n&&s[i][j+1]!='#')
					nextr.d[i][j+1]=1;
				else
					nextr.d[i][j]=1;
			}
		}
	}
	if (dfs(x,y,nextr))
		return 1;
	memset(nextr.d,0,sizeof(nextr.d));
	for (i=0;i<m;i++)
	{
		for (j=0;j<n;j++)
		{
			if (r.d[i][j]==1)
			{
				if (j>0&&s[i][j-1]!='#')
					nextr.d[i][j-1]=1;
				else
					nextr.d[i][j]=1;
			}
		}
	}
	if (dfs(x,y,nextr))
		return 1;
	return 0;
}
void cal(int k,int x,int y)
{
	int c,i,j;
	printf("%d: ",k);
	memset(d,0,sizeof(d));
	dfs2(x,y);
	c=0;
	for (i=0;i<m;i++)
		for (j=0;j<n;j++)
			if (d[i][j]==1)
				c++;
	printf("%d ",c);
	node r;
	memcpy(r.d,d,sizeof(d));
	h.clear();
	if (dfs(x,y,r))
		printf("Lucky\n");
	else
		printf("Unlucky\n");
	return;
}
int main()
{
	int t,tt,i,j,k;
	scanf("%d",&t);
	for (tt=1;tt<=t;tt++)
	{
		cerr<<tt<<endl;
		scanf("%d %d",&m,&n);
		for (i=0;i<m;i++)
			scanf("%s",s[i]);
		printf("Case #%d:\n",tt);
		for (k=0;k<=9;k++)
		{
			for (i=0;i<m;i++)
			{
				for (j=0;j<n;j++)
					if (s[i][j]==k+48)
						break;
				if (j!=n)
					break;
			}
			if (i!=m)
				cal(k,i,j);
		}
	}
	return 0;
}