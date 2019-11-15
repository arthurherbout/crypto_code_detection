#include <iostream>
using namespace std;

int main()
{
	int T,n,Case=1;
	freopen("D-large.in","r",stdin);
	freopen("ansd.out","w",stdout);
	scanf("%d",&T);
	int i,a;
	int ans;
	while(T--)
	{
		scanf("%d",&n);
		ans=0;
		for(i=1;i<=n;i++)
		{
			scanf("%d",&a);
			if(a==i) ans++;
		}
		printf("Case #%d: %d.000000\n",Case++,n-ans);
	}
	return 0;
}