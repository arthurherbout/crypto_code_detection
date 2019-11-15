#include <cstdio>
#include <iostream>
#include <cstring>
using namespace std;

int main()
{
	freopen("C-small-attempt0.in","r",stdin);
	freopen("C.out","w",stdout);
	int _,cases=1;
	scanf("%d",&_);
	while(_--)
	{
		int N,L,H;
		scanf("%d%d%d",&N,&L,&H);
		int num[105];
		for(int i=0;i<N;i++)
			cin>>num[i];
		int res=-1;
		for(int v=L;v<=H&&res==-1;v++)
		{
			bool f=true;
			for(int i=0;i<N&&f;i++)
			{
				if(v%num[i]==0) continue;
				if(num[i]%v==0) continue;
				f=false;
				break;
			}
			if(f) res=v;
		}
		printf("Case #%d: ",cases++);
		if(res==-1) puts("NO");
		else printf("%d\n",res);
	}
	return 0;
}