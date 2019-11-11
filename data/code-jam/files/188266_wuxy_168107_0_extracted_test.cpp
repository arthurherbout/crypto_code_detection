#include <iostream>
using namespace std;
char str[64];
bool m[1000];
bool dfs(int base,int num)
{
	 itoa(num,str,base);
	 int len=strlen(str);
	 int k=0;
	 int i,j;
	 for (i=0;i<len;i++)
	 {
		 j=str[i]-'0';
		 k=k+j*j;
	 }
	 if (m[k])
		 return false;
	 m[k]=true;
	 if (k==1)
		 return true;
	 return dfs(base,k);
}

int main()
{
	freopen("in.txt","r",stdin);
	freopen("out.txt","w",stdout);
	int i,j,k,t,g,len,ans,n,a[10];
	scanf("%d\n",&t);
	for (i=0;i<t;i++)
	{
		scanf("%[^\n]\n",str);
		memset(a,0,sizeof(a));
		len=strlen(str);
		n=0;
		for (k=0;k<len;k+=2)
		{
			sscanf(str+k,"%d",&g);
			a[n]=g;
			n++;
			if (n==10)
				k++;
		}
		for (ans=2;;ans++)
		{
			for (k=0;k<n;k++)
			{
				memset(m,0,sizeof(m));
				if (!dfs(a[k],ans))
					break;
			}
			if (k>=n)
				break;
		}
		printf("Case #%d: %d\n",i+1,ans);
	}
	return 0;
}