#include <iostream>
#include <algorithm>
using namespace std;
int main()
{
	freopen("0.in","r",stdin);
	freopen("0.out","w",stdout);
	int cas=0,t,i;
	int n;
	int a[50];
	int len;
	scanf("%d",&t);
	while(t--)
	{
		cas++;
		scanf("%d",&n);
		len=0;
		while(n)
		{
			a[len++]=n%10;
			n/=10;
		}
		for(i=0;i<len/2;i++)swap(a[i],a[len-1-i]);
		printf("Case #%d: ",cas);
		bool flag=false;
		while(next_permutation(a,a+len))
		{
			for(i=0;i<len-1;i++)printf("%d",a[i]);
			printf("%d\n",a[i]);
			flag=true;
			break;
		}
		if(!flag)
		{
			a[len++]=0;
			sort(a,a+len);
			while(next_permutation(a,a+len))
				if(a[0]>0)break;
			for(i=0;i<len-1;i++)printf("%d",a[i]);
			printf("%d\n",a[i]);
		}
	}
	return 0;
}
