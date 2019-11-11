#include<cstdio>


int main()
{
	int t,a,b,c,d,n,m,high;
	scanf("%d",&t);
	int arr[101][101];
	for(a=1;a<=t;a++)
	{
		scanf("%d%d",&n,&m);
		for(b=0;b<n;b++)
		{
			for(c=0;c<m;c++)
			{
				scanf("%d",&arr[b][c]);
			}
		}
		
		//input complete
		
		for(b=0;b<n;b++)
		{
			high =0;
			for(c=0;c<m;c++)
			{
				if(arr[b][c]>high)
					high=arr[b][c];
			}
			arr[b][100]=high;
		}
		
		for(b=0;b<m;b++)
		{
			high =0;
			for(c=0;c<n;c++)
			{
				if(arr[c][b]>high)
					high=arr[c][b];
			}
			arr[100][b]=high;
		}
		d=0;
		for(b=0;b<n;b++)
		{
			for(c=0;c<m;c++)
			{
				if(arr[b][100]<arr[100][c])
				{
					high = arr[b][100];
				}
				else 
				{
					high = arr[100][c];
				}
				
				if(arr[b][c]!=high)
				{
					d=1;
					break;
				}
			}
		}
		if(d==0)
		{
			printf("Case #%d: YES\n",a);
		}
		else
		{
			printf("Case #%d: NO\n",a);
		}
		
	}
	
	
	return 0;
}
