#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

enum state
{
	OFF_UNPOWERED,
	OFF_POWERED,
	ON_UNPOWERED,
	ON_POWERED
};

int main()
{
	ifstream inf("A-small-attempt1.in");
	// ifstream inf("A-large.in");

	int t;
	int n;
	int k;
	vector<state> snappers;

	// cin >> t;
	inf >> t;

	for (int i = 0; i < t; i++)
	{
		// cin >> n >> k;
		inf >> n >> k;

		snappers.clear();

		snappers.push_back(OFF_POWERED);

		for (int j = 1; j < n; j++)
		{
			snappers.push_back(OFF_UNPOWERED);
		}

		for (int j = 0; j < k; j++)
		{
			switch (snappers.at(0))
			{
			case OFF_POWERED:
				snappers.at(0) = ON_POWERED;
				break;
			case ON_POWERED:
				snappers.at(0) = OFF_POWERED;
				break;
			}

			for (unsigned int s = 1; s < snappers.size(); s++)
			{
				switch (snappers.at(s))
				{
				case OFF_UNPOWERED:
					switch (snappers.at(s - 1))
					{
					case ON_POWERED:
						snappers.at(s) = OFF_POWERED;
						break;
					}
					break;
				case OFF_POWERED:
					switch (snappers.at(s - 1))
					{
					case OFF_UNPOWERED:
						snappers.at(s) = ON_UNPOWERED;
						break;
					case OFF_POWERED:
						snappers.at(s) = ON_UNPOWERED;
						break;
					case ON_UNPOWERED:
						snappers.at(s) = ON_UNPOWERED;
						break;
					case ON_POWERED:
						snappers.at(s) = ON_POWERED;
						break;
					}
					break;
				case ON_UNPOWERED:
					switch (snappers.at(s - 1))
					{
					case ON_POWERED:
						snappers.at(s) = ON_POWERED;
						break;
					}
					break;
				case ON_POWERED:
					switch (snappers.at(s - 1))
					{
					case OFF_UNPOWERED:
						snappers.at(s) = OFF_UNPOWERED;
						break;
					case OFF_POWERED:
						snappers.at(s) = OFF_UNPOWERED;
						break;
					case ON_UNPOWERED:
						snappers.at(s) = OFF_UNPOWERED;
						break;
					case ON_POWERED:
						snappers.at(s) = OFF_POWERED;
						break;
					}
					break;
				}
			}
		}

		cout << "Case #" << i + 1 << ": ";

		if (snappers.at(snappers.size() - 1) == ON_POWERED)
		{
			cout << "ON";
		}
		else
		{
			cout << "OFF";
		}

		cout << "\n";
	}
}

