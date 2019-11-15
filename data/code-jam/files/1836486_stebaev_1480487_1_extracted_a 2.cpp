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
		vector<pair<int, int> > mas(n);
		
		int sum = 0;
		for(int i=0; i<n; i++)
		{
			cin>>mas[i].first;
			sum+=mas[i].first;
			mas[i].second = i;
		}
		sort(mas.begin(), mas.end());
		long double min;		
		int _sum = 0;
		for(int k = 0; k<n; k++)
		{
			_sum += mas[k].first;
			min = (_sum+sum)/(long double)(k+1);
			if(k==n-1 || mas[k+1].first>min) break;
		}
		for(int i=0; i<n; i++)
			swap(mas[i].first, mas[i].second);
		sort(mas.begin(), mas.end());
		printf("Case #%d:", t);
		double ch = 0;
		for(int i=0; i<n; i++)
		{
			cout<<' ';
			if(min>mas[i].second) 
				{
					printf("%f", (min-mas[i].second)*100/sum);
					ch+=(min-mas[i].second)*100/sum;
			}
			else printf("%f", 0.0);
		}
		cout<<endl;
		//cout<<ch<<endl;
	}
}

