#include <iostream>
using namespace std;
int a[128],l[128],r[128],all[128];
int m[128];
int ss[128];
int p,q,ans,tot;
void dfs(int dep)
{
	if (dep==q)
	{
		if (tot<ans)
			ans= tot;
		return;
	}
	int i,j,n,kk;
	for (i=0;i<q;i++)
		if (m[a[i]]==1)
		{
			n=a[i];
			for (j=n-1;j>0;j--)
			{
				if (m[j]==0)
					break;
			}
			kk=n-j-1;
			for (j=n+1;j<=p;j++)
			{
				if (m[j]==0)
					break;
			}
			kk=kk+j-n-1;
			m[n]=0;
			tot+=kk;
			dfs(dep+1);
			tot-=kk;
			m[n]=1;
		}

}
int main()
{
	freopen("in.txt","r",stdin);
	freopen("out.txt","w",stdout);
	int i,j,k,t,len,n,tt;
	scanf("%d\n",&tt);
	for (t=1;t<=tt;t++)
	{
		scanf("%d%d\n",&p,&q);
		for (i=1;i<=p;i++)
			m[i]=1;
		m[0]=m[p+1]=0;
		for (i=0;i<q;i++)
		{
			scanf("%d",a+i);
		}
		ans=p*q;
		tot=0;
		dfs(0);
		printf("Case #%d: %d\n",t,ans);
	}
	return 0;
}