// CandySplitting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;

long patricadd(long a, long b)
{
	return a^b;
}
int _tmain(int argc, _TCHAR* argv[])
{
	int testcases;
	cin >> testcases;

	for(int i=0;i<testcases;++i)
	{
		int num;
		cin >> num;

		int smallest = INT_MAX;
		long sum = 0;
		int regsum = 0;

		for(int j=0 ; j<num ; ++j)
		{
			int val;
			cin >> val;
			sum ^= val;
			regsum += val;
			if(val < smallest) smallest = val;
		}
		if(sum)
			cout << "Case #" << i+1 << ": NO" <<endl;
		else
			cout << "Case #" << i+1 << ": "<< regsum - smallest << endl;
	}
	return 0;
}