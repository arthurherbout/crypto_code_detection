#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	// ifstream inf("in3.txt");
	// ifstream inf("C-small-attempt0.in");
	ifstream inf("C-large.in");
	ofstream outf("out3.txt");

	int cases;
	string s;
	string welcome = "welcome to code jam";
	int sequences[19][500];

	inf >> cases;
	getline(inf, s);

	for (int i = 0; i < cases; i++)
	{
		for (int j = 0; j < welcome.size(); j++)
		{
			for (int k = 0; k < s.size(); k++)
			{
				sequences[j][k] = 0;
			}
		}

		getline(inf, s);

		for (int j = 0; j < welcome.size(); j++)
		{
			for (int k = 0; k < s.size(); k++)
			{
				if (welcome[j] == s[k])
				{
					if (j == 0 && k == 0)
					{
						sequences[j][k] = 1;
					}
					else if (j > 0 && k == 0)
					{
						sequences[j][k] = sequences[j - 1][k];
					}
					else if (j == 0 && k > 0)
					{
						sequences[j][k] = sequences[j][k - 1] + 1;
					}
					else
					{
						sequences[j][k] = sequences[j][k - 1] + sequences[j - 1][k];
					}
				}
				else
				{
					if (k == 0)
					{
						sequences[j][k] = 0;
					}
					else
					{
						sequences[j][k] = sequences[j][k - 1];
					}
				}

				sequences[j][k] %= 10000;
			}
		}

		/*
		for (int j = 0; j < welcome.size(); j++)
		{
			for (int k = 0; k < s.size(); k++)
			{
				cout << sequences[j][k] << " ";
			}

			cout << "\n";
		}

		cout << "\n";
		*/

		int answer = sequences[welcome.size() - 1][s.size() - 1] % 10000;

		outf << "Case #" << i + 1 << ": ";

		if (answer < 10)
		{
			outf << "000";
		}
		else if (answer < 100)
		{
			outf << "00";
		}
		else if (answer < 1000)
		{
			outf << "0";
		}
		
		outf << answer << "\n";
	}
}