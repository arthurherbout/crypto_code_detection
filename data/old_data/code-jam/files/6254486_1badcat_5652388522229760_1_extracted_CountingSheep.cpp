#include <iostream>
#include <set>

using namespace std;

typedef set<int> is;
typedef unsigned long long ull;

const int AS = 10;
void initArray(bool arr[]);
void getDigits(is &s, ull num);


int main()
{

	bool arr[AS];
	int count, dig_c;


	cin >> count;
	for (int i = 1; i <= count; ++i)
	{
		ull pn, sn;
		dig_c = 0;
		cin >> sn;
		pn = sn;
		initArray(arr);
		ull res = 0;

		for (ull i = 1; i < ULLONG_MAX; ++i)
		{
			res = sn * i;
			// todo: check second condition for overflow.
			if (res == 0 || res < pn || ( (res - pn) < sn && i > 1) )
			{
				res = 0;
				break;
			}
			pn = res;
			is s;
			getDigits(s, res);
			for (auto n : s)
			{
				if (arr[n]) continue;
				arr[n] = true;
				dig_c++;
			}
			if (dig_c >= 10) break;
		}
		
		if (res == 0)
			cout << "Case #" << i << ": " << "INSOMNIA" << endl;
		else
			cout << "Case #" << i << ": " <<  res << endl;
	}

		// cin >> num_items;
		// pv items;
		// items.reserve(num_items);
		// for (int j = 1; j < num_items; ++j)
		// {
		// 	cin >> it;
		// 	items.push_back(pair<int,int>(it, j));
		// }
		// sort(items.begin(), items.end());
		
		//cout << "Case #" << i << ": " << (n+m)  << " " << (n*m) << endl;
}

void initArray(bool arr[])
{
	for (int i = 0; i < AS; ++i)
		arr[i] = false;
}
// Assume input number if more than 0
void getDigits(is &s, ull num)
{
	if (num == 0) cout << "Error" << endl;
	while (num)
	{
		s.insert(num % AS);
		num /= AS;
	}

}