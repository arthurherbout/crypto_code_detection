#include<iostream>
#include<cstdio>
#include<cctype>
#include<cmath>
#include<cstdlib>
#include<algorithm>
#include<vector>
#include<string>
#include<list>
#include<deque>
#include<map>
#include<set>
#include<queue>
#include<stack>
#include<utility>
#include<sstream>
#include<cstring>
using namespace std;
 
typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> pii;

#define PB push_back
#define FORE(i,t) for(typeof(t.begin())i=t.begin();i!=t.end();++i)

int t[2][2002][2002];

int malted[2002],toc[2002],special[2002];

int main()
{
	int Z;
	scanf("%d",&Z);
	for(int z=1;z<=Z;++z)
	{
		printf("Case #%d: ",z);
		int n,m;
		scanf("%d%d",&n,&m);
		for(int i=0;i<m;++i)
			for(int j=1;j<=n;++j)
				t[0][i][j]=t[1][i][j]=0;
		for(int i=0;i<m;++i)
		{
			special[i]=-1;
			int k;
			scanf("%d",&k);
			while(k--)
			{
				int x,y;
				scanf("%d%d",&x,&y);
				t[y][i][x]=1;
				//printf("y=%d i=%d x=%d\n",y,i,x);
				if(y==1)
					special[i]=x;
			}
		}
		for(int i=1;i<=n;++i)
			malted[i]=0;
		for(;;)
		{
			bool change=false,cant=false;
			for(int i=1;i<=n;++i)
				toc[i]=false;
			for(int i=0;i<m;++i)
			{
				bool any=false;
				for(int j=1;j<=n;++j)
				{
					if(t[malted[j]][i][j])
					{
						//printf(" (%d %d)\n",i,j);
						any=true;
						break;
					}
				}
				//printf("i=%d any=%d\n",i,(int)any);
				if(!any)
				{
					if(special[i]==-1)
					{
						cant=true;
						break;
					}
					else
					{
						toc[special[i]]=true;
						change=true;
					}
				}
			}
			if(cant)
			{
				puts("IMPOSSIBLE");
				break;
			}
			if(!change)
			{
				for(int i=1;i<=n;++i)
					printf("%d ",malted[i]);
				puts("");
				break;
			}
			for(int i=1;i<=n;++i)
				if(toc[i])
					malted[i]=true;
		}
	}
}
