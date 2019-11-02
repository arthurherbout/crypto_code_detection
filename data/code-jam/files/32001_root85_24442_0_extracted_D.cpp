#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>

#pragma comment(linker, "/STACK:16777216")

using namespace std;

void main()
{

	ifstream in("D-small.in");
	ofstream out("D.out");

	int tc;
	in >>tc;
	for (int cc = 0;cc < tc;cc++)
	{
		int k;
		string s;
		in >>k >>s;

		vector <int> perm(k);
		for (int i = 0;i < k;i++)
			perm[i] = i;

		int res = INT_MAX;
		do
		{
			string cur = s;
			for (int b = 0;b < s.length();b += k)
			{
				for (int i = 0;i < k;i++)
					cur[b + i] = s[b + perm[i]];
			}

			cur += "-";
			//cur = "-" + cur;
			int loc = 0;
			for (int i = 1;i < cur.length();i++)
			{
				if (cur[i - 1] != cur[i])
					loc++;
			}

			res = min(res, loc);
			//cout <<cur <<endl;			
		}
		while (next_permutation(perm.begin(), perm.end()));

		out <<"Case #"<<cc + 1<<": "<<res<<endl;
	}
}
