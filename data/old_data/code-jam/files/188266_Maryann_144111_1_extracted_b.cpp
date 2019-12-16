#include<stdio.h>
#include<queue>
#include<utility>
using namespace std;
int n,m;
long long int a[40][40];
int s[40][40],t[40][40],w[40][40];
bool inq[40][40];
queue<pair<int,int> > q;
const int d[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
long long int Mod(long long int x,long long int y)
{
	x%=y;
	if(x<0)x+=y;
	return x;
}
long long int Div_down(long long int x,long long int y)
{
	long long int r=x/y;
	while(r*y>x)r--;
	return r;
}
long long int calct(int ax,int ay,int bx,int by,long long int now)
{
	long long int ns=s[ax/2][ay/2],ew=w[ax/2][ay/2],tt=t[ax/2][ay/2],tot=ns+ew;
	if(ax==bx)
	{
		if((ay+1)/2==(by+1)/2)
			return now+2;
		//cross the east-west street
		now-=tt;
		if(Mod(now,tot)>=ns)
			return now+tt+1;
		return Div_down(now,tot)*tot+ns+1+tt;
	}
	else
	{
		if((ax+1)/2==(bx+1)/2)
			return now+2;
		//cross the north-south street
		now-=tt;
		if(Mod(now,tot)<ns)
			return now+tt+1;
		return Div_down(now,tot)*tot+ns+ew+1+tt;
	}
}
void spfa()
{
	while(!q.empty())q.pop();
	a[0][0]=0;
	q.push(make_pair(0,0));
	inq[0][0]=true;
	while(!q.empty())
	{
		int x=q.front().first,y=q.front().second,ux,uy;
		long long int newt;
		q.pop();
		inq[x][y]=false;
		for(int i=0; i<4; i++)
		{
			ux=x+d[i][0],uy=y+d[i][1];
			if(ux<0 || ux>=2*n || uy<0 || uy>=2*m)continue;
			newt=calct(x,y,ux,uy,a[x][y]);
			if(a[ux][uy]==-1 || newt<a[ux][uy])
			{
				a[ux][uy]=newt;
				if(!inq[ux][uy])
				{
					inq[ux][uy]=true;
					q.push(make_pair(ux,uy));
				}
			}
		}
	}
}
int main()
{
	int _,__;
	scanf("%d",&_);
	for(__=1; __<=_; __++)
	{
		scanf("%d%d",&n,&m);
		for(int i=n-1; i>=0; i--)
			for(int j=0; j<m; j++)
			{
				scanf("%d%d%d",&s[i][j],&w[i][j],&t[i][j]);
				t[i][j]%=(s[i][j]+w[i][j]);
			}
		memset(a,-1,sizeof(a));
		spfa();
		printf("Case #%d: %I64d\n",__,a[2*n-1][2*m-1]);
	}
	return 0;
}
