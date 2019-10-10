#include<cstdio>
#include<iostream>
#include<string>
#include<vector>
#include<utility>
using namespace std;
#define MAX 1000001

int arr[MAX];

int main()
{
	int cases,caseno=1;
	int k,n,i;
	int indi[100];
	
	cin>>cases;

	while(cases--)
	{
		memset(arr,0,sizeof(arr));
		cin>>k>>n;
		for(i=0;i<n;i++)
			cin>>indi[i];
		
		int cnt=1;
		int p=0;
		int ax=0;
		while(cnt<=k)
		{
			if(!arr[ax])
			{
				p++;
				if(p==cnt)
				{
					arr[ax]=cnt;
					cnt++;
					p=0;
				}
			}
			ax++;
			if(ax==k) ax=0;
		}
					
		
		cout<<"Case #"<<caseno++<<":";
		for(i=0;i<n;i++)
			cout<<" "<<arr[indi[i]-1];
		cout<<endl;
	}
	return 0;
}	

