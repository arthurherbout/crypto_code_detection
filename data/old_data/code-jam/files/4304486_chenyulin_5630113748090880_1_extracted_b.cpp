#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

vector<int>  in[51];
map<int,int> data;
int wid;
int num[2501];
int cnt[2501];
int main()
{
	int tot;
	scanf("%d",&tot);
	for (int k = 0; k < tot; ++k)
	{
		scanf("%d",&wid);
		int t =0;
		for (int i = 0; i < 2*wid-1; ++i)
		{	
			for (int j = 0; j < wid ; ++j)
			{	
				int x ;
				scanf("%d",&x);
				//printf("%d\n",x );
				if (data[x]==0)
				{
					data[x]=1;
				}
				else
					data[x]++;
			}
		}
		int p[52];
		int p1=0;
		for (auto i = data.begin(); i !=data.end() ; ++i)
		{
			if (((*i).second%2)!=0)
			{
				p[p1]=(*i).first;
				p1++;
			}
		}
		for (auto i = data.begin(); i !=data.end() ; ++i)
		{
			(*i).second=0;
		}
		sort(p,p+wid);
		//printf("%d =%d\n",p1,wid );
		printf("Case #%d:",k+1);
		for (int i = 0; i < p1; ++i)
		{
			printf(" %d", p[i]);
		}
		printf("\n");
	}
	
return 0;
}