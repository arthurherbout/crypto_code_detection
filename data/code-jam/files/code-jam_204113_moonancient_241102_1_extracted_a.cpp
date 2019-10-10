#include <iostream>
#include <algorithm>
using namespace std;
#define MAXN 50

int n;
int last[MAXN];
int ans;
int caseN;

int main()
{
	cin>>caseN;
	for (int caseI=1;caseI<=caseN;caseI++)
	{
		cin>>n;
		for (int i=0;i<n;i++)
		{
			last[i]=-1;
			for (int j=0;j<n;j++)
			{
				char ch;
				cin>>ch;
				if (ch=='1') last[i]=j;
			}
		}
		ans=0;
		for (int i=0;i<n;i++)
		{
			int temp=last[i];
			for (int j=i;;j++)
			{
				swap(temp,last[j]);
				if (temp<=i) break;
				ans++;
			}
			last[i]=temp;
		}
		cout<<"Case #"<<caseI<<": "<<ans<<endl;
	}
	return 0;
}


			
