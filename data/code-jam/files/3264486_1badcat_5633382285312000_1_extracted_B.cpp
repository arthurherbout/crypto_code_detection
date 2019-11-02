#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

typedef unsigned long long llu;
typedef vector<int> vi;

bool isTidy(vi& arr);
bool getMaxTidy(vi& arr);


int main()
{
	int N, casen;
	llu num;

	cin >> N;
	casen = 1;
	
	while (N > 0)
	{
		cin >> num;
		vi arr;
		while (num > 0)
		{
			int digit = num%10;
			arr.push_back(digit);
			num /= 10;
		}
		
		cout << "Case #" << casen << ": ";
		getMaxTidy(arr);	
		cout << endl;
		++casen;
		--N;
	}

	return 0;
}


bool isTidy(vi& arr)
{
	for (int i = 0; i < arr.size() - 1 ; ++i)
	{
		if (arr[i] < arr[i+1]) return false;
	}
	return true;
}

bool getMaxTidy(vi& arr)
{
	for (int i = 0; i < arr.size() - 1 ; ++i)
	{
		if (arr[i] < arr[i+1])
		{
			arr[i] = 9;
			arr[i+1] -= 1;

			for (int j = i; j >=0; --j)
				arr[j] = 9;
		}
	}
	int prev = 0;
	for (int i = arr.size() - 1; i >= 0; --i)
	{
		if (arr[i] != 0 || (arr[i] == 0 && prev != 0) )
		{
			cout << arr[i];
		}
		prev = arr[i];
	}
	return true;
}