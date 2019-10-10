// GoroSort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <map>
#include <vector>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int testcases;
	cin >> testcases;

	for(int i =1; i<=testcases; ++i)
	{
		int num;
		map<int,int> numbers;

		cin >> num;
		for(int j=1;j<=num;++j)
		{
			int element;
			cin >> element;

			if(j != element)
			numbers[j] = element;
		}
		double hits = 0.0;
		while(!numbers.empty())
		{
			vector<int> v;
			int first = (*numbers.begin()).first;
			int next = (*numbers.begin()).second;
			v.push_back(first);
			while(first!=next)
			{
				v.push_back(next);
				next = numbers[next];
			}
			hits += v.size();
			for(vector<int>::iterator it = v.begin(); it != v.end(); ++it)
			numbers.erase(numbers.find(*it));
		}
		cout.setf(ios::fixed, ios::floatfield);
		cout.precision(6);
		cout << "Case #"<< i << ": "<< hits<<endl;
	}
	return 0;
}

