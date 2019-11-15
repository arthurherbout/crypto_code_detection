// a.cpp : Defines the entry point for the console application.
//

#include <iostream>
using namespace std;
#include <string>
#include <set>
#include <string.h>
#include <vector>
#include <algorithm>

void main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int T;
	cin>>T;
	for(int t=1; t<=T; t++)
	{
		int n;
		cin>>n;
		int mas[505];
		for(int i=0; i<n; i++)
			cin>>mas[i];
		sort(mas, mas+n);
		int max = 1<<n;
		bool good = false;
		int s1, s2;
		for(s1 = 1; s1<max && !good; s1++)
		{
			int sum1 = 0;
			for(int i=0; i<n; i++)
				if(1<<i & s1)
					sum1+=mas[i];
			for(s2 = 1; s2<max && !good; s2++)
				if(s1&s2) continue;
				else
				{
					int sum2 = 0;
					for(int i=0; i<n; i++)
						if(1<<i & s2)
							sum2+=mas[i];
					if(sum2==sum1)
					{
						good = true;
						break;
					}
				};
			if(good) break;

		}
		printf("Case #%d:\n", t);
		if(good)
		{
		bool f = true;		
		for(int i=0; i<n; i++)		
			if(1<<i & s1)
			{
				if(f) f = false;
				else cout<<' ';
				cout<<mas[i];
			}		
		cout<<endl;
		f = true;
		for(int i=0; i<n; i++)		
			if(1<<i & s2)
			{
				if(f) f = false;
				else cout<<' ';
				cout<<mas[i];
			}		
		cout<<endl;
		}
		else cout<<"Impossible\n";
	}
}

