#include<iostream>
#include<string>
#include<algorithm>
#include<queue>
#include<map>
#include<cmath>
using namespace std;
const double pi=3.141592653589793238462643383279;
char mat[10][10];
char mat1[10][10];
int m,n;
bool f;
char v[10];
int num[10];
void solve(int t)
{
	if(f)return;
	if(t>n)
	{
		bool ff=1;
		for(int i=1;i<=n;i++)
		{
			for(int j=i+1;j<=n;j++)
			{
				if(mat1[i][j]!=mat[num[i]][num[j]])
				{
					f=0;return;
				}
			}
		}
		f=1;
		//for(int i=1;i<=n;i++)
		//	printf("%d -> %d\n",i,num[i])
;		return;
	}
	else
	{
		for(int i=1;i<=m;i++)
		{
			if(f)return;
			if(v[i])continue;
			int cc=num[t];
			num[t]=i;
			v[i]=1;
			solve(t+1);
			v[i]=0;
			num[t]=cc;
			if(f)return;
		}
	}
}
int main()
{
//	priority_queue
	freopen("1.txt","r",stdin);
	freopen("2.txt","w",stdout);
	int zu;
	scanf("%d",&zu);
	for(int Ca=1;Ca<=zu;Ca++)
	{
		printf("Case #%d: ",Ca);
		scanf("%d",&m);
		int x,y;
		memset(mat,0,sizeof(mat));
		memset(mat1,0,sizeof(mat1));
		for(int i=1;i<m;i++)
		{
			scanf("%d%d",&x,&y);
			mat[x][y]=mat[y][x]=1;
		}
		scanf("%d",&n);
		for(int i=1;i<n;i++)
		{
			scanf("%d%d",&x,&y);
			mat1[y][x]=mat1[x][y]=1;
		}
		f=0;
		memset(v,0,sizeof(v));
		solve(1);
		if(f)
			puts("YES");
		else
			puts("NO");
	}
}