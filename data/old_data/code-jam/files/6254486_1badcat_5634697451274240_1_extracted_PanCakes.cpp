#include <iostream>

using namespace std;

const char sad = '-';
int main()
{
	int count;
	string st;

	cin >> count;
	for (int i = 1; i <= count; ++i)
	{
		cin >> st;
		char p = st[0];
		int flips = 0;
		for (int i = 1; i < st.size(); ++i)
		{
			if (p != st[i]) flips++;
			p = st[i];
		}

		// One last flip if all pancakes ended up sad side up.
		if (p == sad) flips++;

		cout << "Case #" << i << ": " << flips << endl;
	}
	
}