#include<stdio.h>
#include<string.h>
#include<algorithm>
using namespace std;
char a[100],tmp;
int N[100];
int main()
{
	freopen("out.txt","w",stdout);
	int T,cs=1,j,i,flag,min,k,ind,len,count;
	scanf("%d",&T);
	getchar();
	while(T--)
	{
		flag=0;
		gets(a);
		len = strlen(a);
		if(strlen(a)==1)
		{
			printf("Case #%d: ",cs);
			printf("%s0\n",a);
		}
		else
		{
			for(i=strlen(a)-1,min=10;i>0;i--)
			{
				if(a[i]>a[i-1])
				{
					flag=3;
					for(j=i,min=100;a[j];j++)
					{
						if(a[j]<min&&a[j]>a[i-1])
						{
							min=a[j];
							ind=j;
						}
					}
					a[ind]=a[i-1];
					a[i-1]=min;
					break;
				}
				
			}
			printf("Case #%d: ",cs);
			if(flag==0)
			{
				for(i=0,k=0;i<len;i++,k++)
				{		
					 N[k]=a[i]-48;
				}
				sort(N,N+k);
				if(N[0]==0)
				{
					for(i=1;i<k;i++)
						if(N[i]>0)
						{
							N[0]=N[i];
							N[i]=0;
							break;
						}
				}
				printf("%c0",N[0]+48);
				for(i=1;i<k;i++)
					printf("%c",N[i]+48);
				printf("\n");
			}
			if(flag==3)
			{
				for(k=0,j=i;a[j];j++,k++)
				{
					N[k]=a[j]-48;
				}
				sort(N,N+k);
				for(j=0;a[i];i++,j++)
					a[i]=N[j]+48;
				printf("%s\n",a);
			}
		}
		cs++;
	}
	return 0;
}