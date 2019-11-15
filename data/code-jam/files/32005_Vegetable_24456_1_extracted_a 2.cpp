#include<iostream>
#include<string>
#include<algorithm>
#include<queue>
#include<map>
#include<cmath>
using namespace std;
const double pi=3.141592653589793238462643383279;
struct node
{
	int x,y,z;
}s[10000];
int main()
{
//	priority_queue
	freopen("1.txt","r",stdin);
	freopen("2.txt","w",stdout);
	int zu;
	scanf("%d",&zu);
	for(int Ca=1;Ca<=zu;Ca++)
	{
		printf("Case #%d:\n",Ca);
		int m,n;
		int x,y;
		string ss;
		char c[234];
		int num=0;
		scanf("%d",&m);
		for(int i=0;i<m;i++)
		{
			scanf("%d%d%s",&x,&y,c);
			ss=c;
			
				s[i].x=x;
				s[i].y=y;
				s[i].z=0;
			if(ss=="BIRD")
			{
				s[i].z=1;
				num++;
			}
			else
			{
				scanf("%s",c);
			}
		}
		//cout<<num<<endl;
		scanf("%d",&n);
		if(num==0)
		{
			int x,y;
			for(int i=0;i<n;i++)
			{
				scanf("%d%d",&x,&y);
				bool f=1;
				for(int j=0;j<m;j++)
				{
					if(s[j].x==x&&s[j].y==y)
					{
						f=0;
						break;
					}
				}
				if(f)
				{
					puts("UNKNOWN");
				}
				else
				{
					puts("NOT BIRD");
				}

			}
		}
		else
		{
			int x1=1000000000,x2=-x1,y1=x1,y2=x2;
			int xd=-1,yd=-1,xx=x1,yy=x1;
			int x,y;
			for(int i=0;i<m;i++)
			{
				if(s[i].z)
				{
					x1=min(x1,s[i].x);
					x2=max(x2,s[i].x);
					y1=min(y1,s[i].y);
					y2=max(y2,s[i].y);
				}
			}
		//	cout<<x1<<" "<<x2<<" "<<y1<<" "<<y2<<endl;
			for(int i=0;i<m;i++)
			{
				if(s[i].z==0)
				{
					if(s[i].x>x2)
						xx=min(xx,s[i].x);
					if(s[i].x<x1)
						xd=max(xd,s[i].x);
					if(s[i].y>y2)
						yy=min(yy,s[i].y);
					if(s[i].y<y1)
						yd=max(yd,s[i].y);
				}
			}
			//cout<<xx<<" "<<yy<<" "<<xd<<" "<<yd<<endl;
			for(int i=0;i<n;i++)
			{
				scanf("%d%d",&x,&y);
				int f=-1;
				for(int j=0;j<m;j++)
				{
					if(s[j].x==x&&s[j].y==y)
					{
						f=s[j].z;
						break;
					}
				}
				if(f>=0)
				{
					if(f)
						puts("BIRD");
					else
						puts("NOT BIRD");
					continue;
				}
				if(x>=x1&&x<=x2&&y>=y1&&y<=y2)
				{puts("BIRD");continue;}
				if(x>=xx||x<=xd||y>=yy||y<=yd)
				{puts("NOT BIRD");}
				else
					puts("UNKNOWN");
			}
		}
	}
}