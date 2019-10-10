#include <stdio.h>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

#define MAX 1100

int g[MAX][MAX];

int cost(int now)
{
	int i;
	int cst[MAX];
	int tmp;
	for(i=1;i<=g[now][0];++i)
	{
		cst[i-1]=cost(g[now][i]);
	}
	sort(cst,cst+g[now][0]);
	tmp=0;
	for(i=0;i<g[now][0];++i)
		if(g[now][0]-i-1+cst[i]>tmp)
			tmp=g[now][0]-i-1+cst[i];
	if(tmp<=g[now][0])
		++tmp;
//	printf("%d(%d):%d\n",now,g[now][0],tmp);
	return tmp;
}

int main()
{
	int t,tcnt;
	int n,m;
	int nid;
	int u,v;
	char tmp[30];
	map<string,int> id;
	int i,j;
	scanf("%d",&t);
	for(tcnt=1;tcnt<=t;++tcnt)
	{
		id.clear();
		nid=0;
		scanf("%d",&n);
		for(i=0;i<n;++i)
		{
			scanf("%s",tmp);
			if(!id.count(string(tmp)))
			{
				g[nid][0]=0;
				u=nid;
				id[string(tmp)]=nid++;
			}
			else
				u=id[string(tmp)];

			scanf("%d",&m);
			for(j=0;j<m;++j)
			{
				scanf("%s",tmp);
				if(tmp[0]<='Z' && tmp[0]>='A')
				{
					if(!id.count(string(tmp)))
					{
						g[nid][0]=0;
						v=nid;
						id[string(tmp)]=nid++;
					}
					else
						v=id[string(tmp)];
					g[u][++g[u][0]]=v;
				}
			}
		}

		printf("Case #%d: %d\n",tcnt,cost(0));

	}
	return 0;
}
				









