#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int main()
{
	int tc,notc,i,j,possible,a[1000],n,l,h,max,min;
	scanf("%d",&notc);
	for(tc=1;tc<=notc;++tc)
	{
		scanf("%d %d %d",&n,&l,&h);
		for(i=0;i<n;++i)
			scanf("%d",&a[i]);
		
		//gpossible=1;
		for(i=l;i<=h;++i)
		{
			possible=1;
			for(j=0;j<n;++j)
			{				
				if (a[j]>i)
				{	max=a[j]; min=i;}
				else {max = i;min=a[j];}
				if (max%min!=0)	
					{possible=0;break;}
			}
			if (possible==1)
				break;
		}
		if (possible==0)
			printf("Case #%d: NO\n",tc);
		else
			printf("Case #%d: %d\n",tc,i);
	}
	return 0;
}