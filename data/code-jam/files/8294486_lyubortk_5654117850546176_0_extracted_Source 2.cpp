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
	ifstream cin("in.txt");
	ofstream cout("out.txt");
	int t;
	cin >> t;

	for (int f = 0; f < t; ++f)
	{
		int n;
		cin >> n;

		vector<int> kek(6);
		for (int i = 0; i < 6; ++i)
			cin >> kek[i];

		int mx = max_element(kek.begin(), kek.end()) - kek.begin();

		list<int> huy;
		while (kek[mx] != 0)
		{
			huy.insert(huy.end(), mx);
			kek[mx]--;
		}

		mx = max_element(kek.begin(), kek.end()) - kek.begin();
		auto it = huy.begin();
		while (kek[mx] != 0)
		{
			++it;
			huy.insert(it, mx);
			kek[mx]--;
		}


		mx = max_element(kek.begin(), kek.end()) - kek.begin();
		it = huy.end();
		while (kek[mx] != 0)
		{
			huy.insert(it, mx);
			--it;
			--it;
			kek[mx]--;
		}

		cout << "Case #" << f + 1 << ": ";

		vector<int> lel;
		for (auto v : huy)
			lel.push_back(v);
		bool hh = 0;
		for (int i = 1; i <= lel.size(); ++i)
			if (lel[i%lel.size()] == lel[i - 1])
			{
				cout << "IMPOSSIBLE";
				hh = 1;
				break;
			}

		if (!hh)
		for (auto v : huy)
		{
			if (v == 0)
				cout << 'R';
			else if (v == 2)
				cout << 'Y';
			else
				cout << 'B';
		}

		cout << endl;
	}
	
	system("pause");

	return 0;
}