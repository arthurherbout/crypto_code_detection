#include<iostream>
using namespace std;

void rellena(int x[],int n)
{
	for(int i=0;i<n;i++)
		x[i] = -1;
}

int sig(int x[],int m,int actual,int n)
{
	int y=1;
	int over=0;

	while(over<n)
	{
		over = 0;
		for(int i=actual;i<n;i++)
		{
			if(x[i]==-1)
			{
				if(m==y)
					return i;
				else
					y++;
			}
			else
			{
				over++;
			}
		}
		actual = 0;
	}
	return -1;
}

void ordena(int x[],int n)
{
	int cuenta=1;
	int i=0;

	rellena(x,n);
	
	while(i!=-1)
	{
			x[i] = cuenta;
			cuenta++;
			i = sig(x,cuenta,i+1,n);
			
	}
}


int main()
{
	int T;
	int K;
	int n;
	int buf;
	int arr[5000];

	cin>>T;
	for(int i=0;i<T;i++)
	{
		cin>>K;

		ordena(arr,K);
		
		cin>>n;
		cout<<"Case #"<<i+1<<":";
		for(int j=0;j<n;j++)
		{
			cin>>buf;
			cout<<" "<<arr[buf-1];
		}
		if(i+1!=T)
			cout<<endl;
	}


	return 0;
}