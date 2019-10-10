#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#include <algorithm>
#include <list>

using namespace std;


int main()
{
#define int long long
	ifstream cin("in.txt");
	ofstream cout("out.txt");
	int t;
	cin >> t;

	for (int f = 0; f < t; ++f)
	{
		int n, q;
		cin >> n >> q;

		vector<pair<int, int>> arr(n);

		for (int i = 0; i < n; ++i)
			cin >> arr[i].first >> arr[i].second;

		vector<double> time(n, 1e18);
		time[0] = 0;

		vector<vector<int>> d(n, vector<int>(n));

		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				cin >> d[i][j];

		cin >> q >> q;

		for (int i = 0; i < n; ++i)
		{
			int kek = 0;
			for (int j = i + 1; j < n; ++j)
			{
				kek += d[j - 1][j];
				if (kek > arr[i].first)
					break;
				time[j] = min(time[j], time[i] + kek / 1.0 / arr[i].second);
			}
		}
		cout << "Case #" << f + 1 << ": ";
		cout.precision(7);
		cout << fixed <<  time[n-1];
		
		cout << endl;
	}
	
	system("pause");

	return 0;
}