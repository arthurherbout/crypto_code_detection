#include <iostream>
using namespace std;
char str[64];
int m[126];
int a[64];
int main()
{
	freopen("in.txt","r",stdin);
	freopen("out.txt","w",stdout);
	int i,j,k,t,len;
	long long ans,g;
	scanf("%d\n",&t);
	for (i=0;i<t;i++)
	{
		scanf("%s\n",str);
		len=strlen(str);
		for (j=0;j<126;j++)
			m[j]=-1;
		if (len==1)
		{
			printf("Case #%d: 1\n",i+1);
			continue;
		}
		for (j=1;j<len;j++)
		{
			if (str[j]!=str[0])
				break;
		}
		if (j>=len)
		{
			g=1;
			for (j=0;j<len;j++)
			{
				g=g*2;
			}
			printf("Case #%d: %I64d\n",i+1,g-1);
			continue;
		}
		if (len==2)
		{
			printf("Case #%d: 2\n",i+1);
			continue;
		}
		m[str[0]]=1;
		a[0]=1;

		for (j=1;j<len;j++)
		{
			if (str[j]!=str[0])
				break;
			a[j]=1;
		}
		a[j]=0;
		m[str[j]]=0;
		k=2;
		for (j++;j<len;j++)
		{
			if (m[str[j]]<0)
			{
				a[j]=k;
				m[str[j]]=k;
				k++;
			}
			else
			{
				a[j]=m[str[j]];
			}
		}
		ans=0;
		g=1;
		for (j=len-1;j>=0;j--)
		{
			ans=ans+a[j]*g;
			g=g*k;
		}
		printf("Case #%d: %I64d\n",i+1,ans);
	}
	return 0;
}