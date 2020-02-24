#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

typedef unsigned int uint;

struct Test
{
	std::vector<uint> pancakes;
	Test():pancakes(){}
};

void getNumCakes(string line, vector<uint>& v)
{
	uint j = 0;
	uint l = 0;
	v.clear();
	for (uint i = 0; i < line.size(); ++i)
	{
		if (line[i] == ' ')
		{
			v.push_back(atoi(line.substr(j,l).c_str()));
			l = 0;
			j = i + 1;
		}
		else
			l++;
	}
	if (line[line.size()-1] != ' ') v.push_back(atoi(line.substr(j,l).c_str()));
};

uint minMinutesOfRest(uint fin, std::vector<uint>& max_heap)
{
	pop_heap(max_heap.begin(), max_heap.end());
	uint max = max_heap.back();
	if (max < 4) return (max + fin);
	uint orig_val = max + fin;
	max_heap.pop_back();
	uint new_d = (max/2);
	max -= new_d;
	max_heap.push_back(new_d);
	max_heap.push_back(max);
	fin++;
	return min(orig_val, minMinutesOfRest(fin, max_heap)); 
};
uint getMinMinutes(Test& t)
{
	vector<uint> max_heap;

	for (uint i = 0; i < t.pancakes.size(); ++i)
	{
		//cout << t.pancakes[i] << endl;
		max_heap.push_back(t.pancakes[i]);
	}
	make_heap(max_heap.begin(), max_heap.end());
	uint fin = 0;
	return minMinutesOfRest(fin, max_heap);

	/*while(max_heap.size())
	{
		pop_heap(max_heap.begin(), max_heap.end());
		// cout << max_heap.back() << endl;
		uint max = max_heap.back();
		if ( (min + 1 + (max-(max/2))) < (min + max))
		{
			max_heap.pop_back();
			uint new_d = (max/2);
			max -= new_d;
			max_heap.push_back(new_d);
			max_heap.push_back(max);

			min++;
		}
		else
		{
			min += max;
			break;
		}
	}
	return min;
	*/
};

int main()
{
string num_tests_str;
	uint num_tests;
	getline(cin, num_tests_str);
	num_tests = atoi(num_tests_str.c_str());
	vector<uint> results;
	results.reserve(num_tests);
	for (uint i = 0; i < num_tests; ++i)
	{
		uint num_dine = 0;
		string num_dine_str;
		getline(cin, num_dine_str);
		num_dine = atoi(num_dine_str.c_str());
		string pcake_str;
		getline(cin, pcake_str);
		Test t;
		getNumCakes(pcake_str, t.pancakes);
		results.push_back(getMinMinutes(t));
	}
	for (uint i = 0; i < results.size(); ++i)
	{
		cout << "Case #" << (i+1) << ": " << results[i] << endl;
	}
	return 0;	
}