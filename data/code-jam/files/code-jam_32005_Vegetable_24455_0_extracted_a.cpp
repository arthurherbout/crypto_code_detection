#include<iostream>
#include<string>
#include<algorithm>
#include<queue>
#include<map>
#include<cmath>
using namespace std;
const double pi=3.141592653589793238462643383279;
int dx[4]={-1,0,1,0};int dy[4]={0,-1,0,1};
//int dx[4]={-1,0,1,0};int dy[4]={0,1,0,-1};
//int seq[12];
int res=0;
int m,n,x,y;
const int inf=1000000;
inline bool ok(int x,int y)
{
	if(x<0||y<0||x>=m||y>=n)return 1;
	return 0;
}
void out(char mat[][5])
{
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(j)
				printf(" ");
			printf("%d",(int)mat[i][j]);
		}
		puts("");
	}
	puts("");
}
void solve(int t,char mat[][5])
{
	if(t>res)
		res=t;
	bool f=1;
	for(int i=0;i<4;i++)
	{
		int tx=x+dx[i];
		int ty=y+dy[i];
		if(ok(tx,ty))continue;
		if(mat[tx][ty]){f=0;break;}
	}
	if(f)
	{res=inf;return;}
	char temp[5][5];
	//char temp1[5][5];
	//char temp1[5][5];
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<n;j++)
		{
			//temp1[i][j]=
				temp[i][j]=mat[i][j];
		}
	}
	//out(temp);
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(i==x&&j==y)continue;
			int index=-1;
			int da=0;
			for(int k=0;k<4;k++)
			{
				int tx=i+dx[k];
				int ty=j+dy[k];
				if(ok(tx,ty))continue;
				if(mat[tx][ty]>da)
				{
					da=mat[tx][ty];index=k;
				}
			}
			if(index==-1)continue;
			int tx=i+dx[index];
			int ty=j+dy[index];
			temp[tx][ty]-=mat[i][j];
			if(temp[tx][ty]<0)temp[tx][ty]=0;
		}
	}
	//out(temp);
	if(temp[x][y]<=0)return;
	for(int i=0;i<4;i++)
	{
		int tx=x+dx[i];
		int ty=y+dy[i];
		if(ok(tx,ty))continue;
		if(temp[tx][ty]==0)continue;
		int dv=mat[x][y];
		char temp1[5][5];
		for(int ii=0;ii<m;ii++)
			for(int jj=0;jj<n;jj++)
				temp1[ii][jj]=temp[ii][jj];
		temp1[tx][ty]-=dv;
		if(temp1[tx][ty]<0)temp1[tx][ty]=0;
		solve(t+1,temp1);
		if(res==inf)return;
	}
	char temp1[5][5];
	for(int ii=0;ii<m;ii++)
			for(int jj=0;jj<n;jj++)
				temp1[ii][jj]=temp[ii][jj];
	solve(t+1,temp1);
	if(res==inf)return;
}
char mat1[5][5];
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
		scanf("%d%d%d%d",&n,&m,&y,&x);
		x--;
		y--;
		//cout<<m<<" "<<n<<" "<<x<<" "<<y<<endl;
		int zzz;
		for(int i=0;i<m;i++)
		{
			for(int j=0;j<n;j++)
			{
				scanf("%d",&zzz);
				mat1[i][j]=zzz;
			}
		}
		//out(mat1);
		res=0;
		solve(0,mat1);
		if(res==inf)
			puts("forever");
		else
		{
			if(res==1)
				printf("1 day\n");
			else

			printf("%d days\n",res);
		}
	}
}