#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

int gcd(const int &a, const int &b);

int main()
{
	int c;
	int n;
	vector<int> events;
	bool match;
	int first_mod;
	string suffix;
	int suffix_i;
	stringstream ss;
	vector<int> diffs;
	int divisor;
	int y;

	scanf("%d", &c);

	for (int i = 0; i < c; i++)
	{
		scanf("%d", &n);

		events.clear();

		for (int j = 0; j < n; j++)
		{
			int t;

			scanf("%d", &t);

			events.push_back(t);
		}

		suffix.clear();
		match = true;

		while (match)
		{
			first_mod = events.at(0) % 10;

			for (unsigned int j = 1; j < events.size(); j++)
			{
				if ((events.at(j) % 10) != first_mod)
				{
					match = false;
				}
			}

			if (match)
			{
				suffix = char('0' + first_mod) + suffix;
			
				for (unsigned int j = 0; j < events.size(); j++)
				{
					events.at(j) /= 10;
				}
			}
		}

		if (!suffix.empty())
		{
			ss.clear();
			ss << suffix;
			ss >> suffix_i;

			if (suffix_i != 0)
			{
				suffix_i = int(pow(10, suffix.size())) - suffix_i;
				
				ss.clear();
				ss << suffix_i;
				ss >> suffix;
			}
		}

		diffs.clear();

		for (unsigned int j = 1; j < events.size(); j++)
		{
			diffs.push_back(abs(double(events.at(j - 1) - events.at(j))));
		}

		if (diffs.size() == 1)
		{
			y = diffs.at(0) - (events.at(0) % diffs.at(0));
		}
		else if (diffs.size() == 2)
		{
			divisor = gcd(diffs.at(0), diffs.at(1));

			y = divisor - (events.at(0) % divisor); 
		}

		printf("Case #%d: ", i + 1);

		if ((y == 1) && suffix.empty())
		{
			printf("0");
		}
		else if (y == 1)
		{
			printf("%s", suffix.c_str());
		}
		else
		{
			printf("%d%s", y, suffix.c_str());
		}

		printf("\n");
	}
}

int gcd(const int &a, const int &b)
{
	if (b == 0)
	{
		return a;
	}
	else
	{
		return gcd(b, a - (b * (a / b)));
	}
}

