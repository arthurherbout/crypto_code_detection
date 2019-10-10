// CandySplitting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int testcases;
	cin >> testcases;

	for(int i=0;i<testcases;++i)
	{
		int num;
		cin >> num;

		long smallest = INT_MAX;
		long sum = 0;
		long regsum = 0;

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