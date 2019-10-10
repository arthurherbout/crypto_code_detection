#include<iostream>
#include<vector>
using namespace std;
int main()
{
	int T,t1=1;
	cin>>T;
	while(T--)
	{
		int v1,v2,flag=0,a,m1[4][4],m2[4][4];
		cin>>v1;
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				cin>>m1[i][j];
			}
		}
		cin>>v2;
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				cin>>m2[i][j];
			}
		}
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				if(m1[v1-1][i]==m2[v2-1][j])
				{
					flag++;
					a=m1[v1-1][i];
				}
			}
		}
		if(flag==0)
		{
			cout<<"Case #"<<t1<<": Volunteer cheated!"<<endl;
		}
		else if(flag==1)
		{
			cout<<"Case #"<<t1<<": "<<a<<endl;
		}
		else
			cout<<"Case #"<<t1<<": Bad magician!"<<endl;

		t1++;
	}
	return 0;
}