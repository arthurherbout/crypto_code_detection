#include<iostream>
#include<algorithm>
using namespace std;

char a[7][10];
int zhong=0;
int m,n;
void dfs(int beg,int end)
{
	if(end==0)
	{
		zhong++;
		return;
	}
	for(int i=beg+1;i<=end;i++)
	{
		int ind=0;
		int x = i;
		bool f=1;
		while(x)
		{
			if(a[ind][x%n])
			{
				f=0;break;
			}
			ind++;
			x/=n;
		}
		if(!f)continue;
x = i;
		//bool f=1;
ind=0;
		while(x)
		{
			a[ind][x%n]=1;
			ind++;
			x/=n;
		}

	dfs(i, end - i);
		
	x=i;
	ind=0;
		while(x)
		{
			a[ind][x%n]=0;
			ind++;
			x/=n;
		}
	}
}
int main()
{
	freopen("1.txt","r",stdin);
	freopen("2.txt","w",stdout);
	int zu;
	cin>>zu;
	for(int cas = 1;cas<=zu;cas++)
	{
		printf("Case #%d: ", cas);
		cin>>m>>n;
		memset(a,0,sizeof(a));
		zhong=0;
		dfs(0,m);
		cout<<zhong<<endl;
	}
}