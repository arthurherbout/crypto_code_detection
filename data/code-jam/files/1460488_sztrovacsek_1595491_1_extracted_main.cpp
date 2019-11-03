#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

int t;
int n;
int s;
int p;



int main()
{
	ifstream f;
	f.open("B.in");
	if (f.fail())
	{
		cout << "B.in not found" << endl;
		return 1;
	}
	f >> t;
	for (int i = 1; i <= t; i++)
	{
		f >> n;
		f >> s;
		f >> p;
		int total;
		int max_nostar;
		int max_star;
		int over_p = 0;
		int exactly_before_p = 0;
		for (int j = 0; j < n; j++)
		{
			f >> total;
			if (total%3 == 0)
			{
				max_nostar = total/3;
				if (total/3 == 0)
				{
					max_star = total/3;
				}
				else
				{
					max_star = total/3 + 1;
				}
			}
			if (total%3 == 1)
			{
				max_nostar = total/3 + 1;
				max_star = total/3 + 1;
			}
			if (total%3 == 2)
			{
				max_nostar = total/3 + 1;
				if (total/3 == 9)
				{
					max_star = total/3 + 1;
				}
				else
				{
					max_star = total/3 + 2;
				}
			}
			if (max_nostar >= p)
			{
				over_p += 1;
			}
			else if ((max_nostar == p-1) && max_star == p)
			{
				exactly_before_p += 1;
			}
		}
		cout << "Case #" << i <<": " << over_p + min(s, exactly_before_p) << endl;
	}
}
