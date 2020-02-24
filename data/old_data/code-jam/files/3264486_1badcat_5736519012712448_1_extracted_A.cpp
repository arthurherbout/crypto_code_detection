#include <iostream>

using namespace std;

int main()
{
	int N, k, casen;
	string pc;
	cin >> N;
	casen = 1;
	while (N > 0)
	{
		int flips = 0;
		bool psbl = true;

		cin >> pc;
		cin >> k;

		for (int i = 0; i < pc.size(); ++i)
		{
			if (pc[i] == '+') continue;
			if ((i+k) > pc.size() ) 
			{
				psbl = false;
				break;
			}
			for (int j = 0; j < k; ++j)
			{
				pc[i + j] = (pc[i + j] == '-') ? '+' : '-';
			}
			++flips;
		}

		cout << "Case #" << casen << ": ";
		psbl ? cout << flips : cout << "IMPOSSIBLE"; 
		cout << endl;

		++casen;
		--N;
	}

	return 0;
}