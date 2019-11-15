#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

using namespace std;

bool is_happy(int attempt, const int base, vector<int> history);

int main()
{
	// ifstream inf("in.txt");
	ifstream inf("A-small-attempt0.in");
	// ifstream inf("A-large.in");
	ofstream outf("out.txt");
	
	int t;
	string s;
	vector<int> bases;
	int base;
	int attempt;
	bool all_happy;

	inf >> t;

	getline(inf, s);

	for (int i = 0; i < t; i++)
	{
		getline(inf, s);
		stringstream ss(s);

		bases.clear();

		while (ss >> base)
		{
			bases.push_back(base);
		}

		attempt = 2;

		while (true)
		{
			all_happy = true;

			for (unsigned int j = 0; j < bases.size(); j++)
			{
				vector<int> history;

				if (!is_happy(attempt, bases[j], history))
				{
					all_happy = false;
					break;
				}
			}

			if (all_happy)
			{
				break;
			}
			else
			{
				attempt++;
			}
		}

		outf << "Case #" << (i + 1) << ": " << attempt << "\n";
	}
}

bool is_happy(int attempt, const int base, vector<int> history)
{
	if (attempt == 1)
	{
		return true;
	}
	else
	{
		for (unsigned int i = 0; i < history.size(); i++)
		{
			if (attempt == history[i])
			{
				return false;
			}
		}

		history.push_back(attempt);

		vector<int> digits;

		while (attempt > 0)
		{
			digits.push_back(attempt % base);
			attempt /= base;
		}

		int sum = 0;

		for (unsigned int i = 0; i < digits.size(); i++)
		{
			sum += (digits[i] * digits[i]);
		}

		return is_happy(sum, base, history);
	}
}