#include<stdio.h>
#include<map>
#include<string>
using namespace std;
class folder
{
public:
	map<string,folder*> Map;
	folder(){}
} *root,*p,*q;
int main()
{
	int _,t,n,m;
	char s[110000];
	scanf("%d",&_);
	for(t=1; t<=_; t++)
	{
		scanf("%d%d",&n,&m);
		int ans=0;
		root=new folder();
		for(int i=0; i<n+m; i++)
		{
			scanf("%s",s);
			p=root;
			for(int j=0;;)
			{
				string w="";
				for(j++; s[j]!=0 && s[j]!='/'; j++)
					w+=s[j];
				if(p->Map.find(w)==p->Map.end())
				{
					p->Map[w]=new folder();
					if(i>=n)
						ans++;
				}
				p=p->Map[w];
				if(s[j]==0)break;
			}
		}
		printf("Case #%d: %d\n",t,ans);
	}
	return 0;
}
