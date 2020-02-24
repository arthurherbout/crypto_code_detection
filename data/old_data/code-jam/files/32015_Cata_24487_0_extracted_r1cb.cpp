#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int st[50];
int n;
string s;

int ugly(long long x)
{
	if (x%2==0 || x%3==0 || x%5==0 || x%7==0)
		return 1;
	else
		return 0;
}

long long nr()
{
	long long x=0;
	long long y=s[0]-'0';
	int sign=1;
	int i;
	for (i=0; i<n-1; i++)
	{
		if (st[i]>0)
		{
			x+=y*sign;
			if (st[i]==1)
				sign=1;
			else
				sign=-1;
			y=0;
		}
		y=y*10+s[i+1]-'0';
	}
	x+=y*sign;
	//cout<<(int)x<<" ";
	return x;
}

int back(int k)
{
	int sum=0;
	for (int i=0; i<3; i++)
	{
		st[k]=i;
		if (k==n-2)
			sum+=ugly(nr());
		else
			sum+=back(k+1);
	}
	return sum;
}

int main()
{
	int N;
	cin>>N;
	for (int test=1; test<=N; test++)
	{
		cin>>s;
		n=s.size();
		int rez;
		if (n>1)
			rez=back(0);
		else
			rez=ugly(s[0]-'0');
	//	cout<<endl;
		cout<<"Case #"<<test<<": "<<rez<<endl;
	}
	return 0;
}
