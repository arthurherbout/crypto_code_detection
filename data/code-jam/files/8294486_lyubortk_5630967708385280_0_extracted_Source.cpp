#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#include <algorithm>

using namespace std;

int main()
{
	ifstream cin("in.txt");
	ofstream cout("out.txt");
	int t;
	cin >> t;

	for (int f = 0; f < t; ++f)
	{
		int d, n;
		cin >> d >> n;

		double t_max = 0;

		while (n--)
		{
			int k, s;
			cin >> k >> s;
			t_max = max(t_max, (d - k) / 1.0 / s);
		}

		cout.precision(7);
		cout << fixed << "Case #" << f + 1 << ": " << d / 1.0 / t_max << endl;
	}
	

	return 0;
}