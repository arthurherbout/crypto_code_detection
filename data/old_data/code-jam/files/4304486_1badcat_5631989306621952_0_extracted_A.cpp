#include <iostream>

using namespace std;

typedef unsigned long long ull;


int main()
{
	int count;

	cin >> count;
	
	for (int i = 1; i <= count; ++i)
	{
		
		string word, res;
		cin >> word;
		res = word[0];
		for (int i = 1; i < word.size(); ++i)
		{
			if (word[i] < res[0])
				res = res + word[i];
			else
				res = word[i] + res;
		}
		cout << "Case #" << i << ": " << res << endl;
	}
	return 0;
}