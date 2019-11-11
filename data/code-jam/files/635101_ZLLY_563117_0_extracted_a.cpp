#include <iostream>
#include <algorithm>
using namespace std;
struct node
{
	char path[105];
	bool exist;
}ans[205];
int cmp(node a,node b)
{
	return strcmp(a.path,b.path)<0;
}

int create(int i,int j){//i´æÔÚ
	int isum=0;
	int k=0;
	while(ans[i].path[k]==ans[j].path[k] && ans[i].path[k] && ans[j].path[k]){
		k++;
	}
	if(ans[j].path[k]!=ans[i].path[k])
	{
		if((ans[j].path[k]=='/' && ans[i].path[k]==0) 
			||(ans[i].path[k]=='/' && ans[j].path[k]==0))
			isum--;
		isum++;

	}
	while(ans[j].path[k])
	{
		if(ans[j].path[k]=='/') isum++;
		k++;
	}
	return isum;
}
int main()
{  
	freopen("read.txt","r",stdin);
	freopen("ans.txt","w",stdout);
	int t,n,m,sum,next,front,nsum,fsum,ni,i;
	scanf("%d",&t);
	for(ni=1;ni<=t;ni++){
		scanf("%d%d",&n,&m);
		for(i=0;i<n;i++)
		{
			scanf("%s",&ans[i].path);
			ans[i].exist=true;
		}
        for(;i<n+m;i++)
		{
			scanf("%s",&ans[i].path);
			ans[i].exist=false;
		}
		sort(ans,ans+n+m,cmp);
		sum=0;
		for(i=n+m-1;i>=0;i--)
		{
			if(ans[i].exist) continue;
            next=i+1;
			front=i-1;
			while(front>-1 && !(ans[front].exist)) front--;
			while(next<n+m && !(ans[next].exist)) next++;
			if(front<=-1 && next>=n+m){
				int k=0;
				while(ans[i].path[k])
				{
                 if(ans[i].path[k]=='/') sum++;
	             k++;
				}
                ans[i].exist=true;
				continue;
			}
			if(front<=-1) fsum=10005;
			else fsum=create(front,i);
			if(next>=n+m) nsum=1005;
			else nsum=create(next,i);
			sum+=(fsum<nsum?fsum:nsum);
			ans[i].exist=true;
		}
		printf("Case #%d: %d\n",ni,sum);
	}
	return 0;
}
