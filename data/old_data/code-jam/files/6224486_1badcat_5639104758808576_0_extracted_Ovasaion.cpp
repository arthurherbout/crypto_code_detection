#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

typedef unsigned int uint;
struct Test
{
	uint s_max;
	vector<uint> pos_num;
	Test():s_max(0), pos_num(){}
};

uint getFriendsNum(Test& t)
{
	uint fn = 0;
	uint run_total = 0;
	// cout << "S Max " << t.s_max << endl;
	for (uint i = 0; i < t.pos_num.size(); ++i)
	{
		//cout << "Pos " << i << " Num " << t.pos_num[i] << endl;
		if (run_total < i)
		{
			uint gap = i - run_total;
			fn += gap;
			run_total += gap;
		}
		run_total += t.pos_num[i];
	}
	return fn;
};

void getTest(string input, Test& t1)
{
	bool ready = false;
	for (uint a = 0; a < input.size(); ++a)
	{
		if (input[a] == ' ')
		{
			t1.s_max = atoi(input.substr(0,a).c_str());
			ready = true;
			continue;
		}	
		if (ready)
		{
			t1.pos_num.push_back(atoi(input.substr(a,1).c_str()));
		}
	}
}

int main()
{
	string num_tests_str;
	uint num_tests;
	getline(cin, num_tests_str);
	num_tests = atoi(num_tests_str.c_str());
	vector<uint> results;
	for (uint i = 0; i < num_tests; ++i)
	{
		string line;
		Test t;
		getline(cin, line);
		getTest(line, t);
		results.push_back(getFriendsNum(t));
	}
	for (uint i = 0; i < results.size(); ++i)
	{
		cout << "Case #" << (i+1) << ": " << results[i] << endl;
	}
	return 0;
}
