#include<stdio.h>
int compare(const void *x,const void *y)
{
	int a=*((int *)x);
	int b=*((int *)y);
	if(a > b)
		return 1;
	return -1;
}
int main()
{
	int t,n;
	int i,j,k;
	int arr[1000][2];
	int temp;
	int count;
	scanf("%d",&t);
	for(i=0;i<t;i++)
	{
		count=0;
		scanf("%d",&n);
		for(j=0;j<n;j++)
			scanf("%d%d",&arr[j][0],&arr[j][1]);
		qsort(arr,n,sizeof(arr[0]),compare);
		//      for(j=0;j<n;j++)
		//              printf("%d %d\n",arr[j][0],arr[j][1]);
		for(j=0;j<n;j++)
		{
			temp=arr[j][1];
			for(k=j;k<n;k++)
				if(temp > arr[k][1])
					count++;
		}
		printf("Case #%d: %d\n",i+1,count);
	}
	return 0;
}
