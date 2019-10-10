#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main()
{
	// ifstream inf("in.txt");
	ifstream inf("B-small-attempt3.in");
	ofstream outf("out.txt");

	int t;
	int n;
	vector<int> digits;
	int swap;
	int swap_index;
	bool done;
	bool done2;

	inf >> t;

	for (int i = 0; i < t; i++)
	{
		inf >> n;

		digits.clear();

		while (n > 0)
		{
			digits.push_back(n % 10);
			n /= 10;
		}

		done = false;

		for (int j = 0; j < digits.size(); j++)
		{
			for (int k = 0; k < j; k++)
			{
				if (digits[j] < digits[k])
				{
					swap = digits[j];
					digits[j] = digits[k];
					digits[k] = swap;
					
					swap_index = j;
					done = true;
					break;
				}
			}

			if (done)
			{
				break;
			}
		}

		if (done)
		{
			done2 = false;

			for (int j = 0; j < swap_index; j++)
			{
				for (int k = j + 1; k < swap_index; k++)
				{
					if (digits[k] > digits[j])
					{
						swap = digits[j];
						digits[j] = digits[k];
						digits[k] = swap;						
					}
				}
			}
		}
		else
		{
			int min_digit = 9;
			int min_index = 0;

			for (int j = 0; j < digits.size(); j++)
			{
				if ((digits[j] > 0) && (digits[j] <= min_digit))
				{
					min_digit = digits[j];
					min_index = j;
				}
			}

			digits.insert(digits.end(), min_digit);
			digits[min_index] = 0;

			for (int j = 0; j < digits.size() - 1; j++)
			{
				for (int k = j + 1; k < digits.size() - 1; k++)
				{
					if (digits[k] > digits[j])
					{
						swap = digits[j];
						digits[j] = digits[k];
						digits[k] = swap;						
					}
				}
			}
		}

		n = 0;

		for (int j = digits.size() - 1; j >= 0; j--)
		{
			if (j < digits.size() - 1)
			{
				n *= 10;
			}

			n += digits[j];
		}

		outf << "Case #" << (i + 1) << ": " << n << "\n";
	}
}