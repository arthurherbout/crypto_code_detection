#include <iostream>
#include "stdio.h"
#include <vector>
using namespace std;

//#define DBGOUT printf
 void DBGOUT(...) { } 

int nCells;
int nMen;
//int nMenNo[200];

int getLeftVal(vector<int> rel, int sel)
{
	int minMax = 0;

	DBGOUT("getleftval .. %d among %d..", sel, rel.size());
	for (int i = 0; i < rel.size(); i++)
	{
		if (rel[i] < sel)
			if (rel[i] > minMax)
				minMax = rel[i];
	}

	DBGOUT("minmax=%d\n", minMax);

	return (sel - minMax - 1);
}

int getRightVal(vector<int> rel, int sel)
{
	int minMax = nCells + 1;

	DBGOUT("getrightval .. %d among %d..", sel, rel.size());
	for (int i = 0; i < rel.size(); i++)
	{
		if (rel[i] > sel)
			if (rel[i] < minMax)
				minMax = rel[i];
	}

	DBGOUT("minmax=%d\n", minMax);

	return (minMax - sel - 1);
}


int getMinGolds(vector<int> numbers, vector<int> released)
{
	int ret;
	int min = 99999999;
	int sel;
	vector<int> rel;
	vector<int> nums;

	if (numbers.size() == 0)
		return 0;

	int left = 0;
	int right = 0;

	for (int i = 0; i < numbers.size(); i++)
	{
		sel = numbers[i];
		DBGOUT("selected %d\n", sel);
		nums = numbers;
		rel = released;
		nums.erase(nums.begin() + i);
		left = getLeftVal(rel, sel);
		DBGOUT("left=%d\n", left);
		right =  getRightVal(rel, sel);
		DBGOUT("right=%d\n", right);
		ret = left + right;
		rel.push_back(sel);
		ret += getMinGolds(nums, rel);
		if (ret < min)
			min = ret;
	}

	return min;
}
			
int main()
{
	int case_cnt;
	int ret;
	vector<int>	menNo;
	vector<int>	released;;

	scanf("%d", &case_cnt);

	for (int i = 0; i < case_cnt; i++)
	{
		scanf("%d %d", &nCells, &nMen);

		//memset(nMenNo, 0, 200);
		menNo.clear();
		released.clear();

		int no;
		for (int j = 0; j < nMen; j++) 
		{
			scanf("%d", &no);
			menNo.push_back(no);
		}
		
		ret = getMinGolds(menNo, released);

		cout << "Case #" << i + 1 << ": " << ret << endl;
	}
}

