// Magicka.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <map>
#include <string>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int testcases;
	cin >> testcases;

	for(int i=1;i<=testcases;++i)
	{
		int numstrings;
		cin >> numstrings;

		map<string, char> combine;
		multimap<char, char> oppose;

		for(int j=0;j<numstrings;++j)
		{
			char first;
			char second;
			char replace;
			cin >> first;
			cin >> second;
			cin >> replace;

			string s;
			s=first;
			s+=second;
			combine[s] = replace;

			s.clear();
			s= second;
			s+= first;
			combine[s] = replace;
		}

		// oppose strings
		cin >> numstrings;

		for(int j=0;j<numstrings;++j)
		{
			char first, second;
			cin >> first >> second;

			oppose.insert(pair<char,char>(first, second));
			oppose.insert(pair<char,char>(second, first));
		}

		// no need of length
		cin >> numstrings;

		string s,op;
		cin >> s;
		op.clear();

		char first, second;
		while(!s.empty())
		{
			if(op.empty())
			{
				first = s[0];
				s.erase(s.begin());
			}
			else
			{
				// take the last element of output string op
				first = op[op.length()-1];
				op.erase(op.length()-1);

			}
			if(!s.empty())
			{
				second = s[0];
				s.erase(s.begin());
			}
			else
			{
				op=first;
				break;
			}

			string chk;
			chk = first;
			chk += second;

			map<string, char>::iterator combineitr = combine.find(chk);
			if( combineitr != combine.end())
			{
				op += (*combineitr).second;
			}
			else
			{
				if(op.empty())
				{
					op = first;
					op += second;
				}
				else
				{
					op += first;
					op += second;
				}

				for(unsigned int k=0; k < op.length();++k)
				{
					multimap<char,char>::iterator it = oppose.find(op[k]);
					if(it != oppose.end() && op.find((*it).second) != string::npos)
					{
						op.clear();
						break;
					}
				}
			}
		}
		cout << "Case #"<< i << ": [";
		for(unsigned int j=1;j<op.length();++j)
			cout<<op[j-1]<<", ";
		if(!op.empty())
			cout<<op[op.length()-1];
		cout << "]"<< endl;
	}
	return 0;
}

