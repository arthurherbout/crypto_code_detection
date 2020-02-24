#include <iostream>

using namespace std;

int main()
{
	freopen("C-small-attempt0.in", "r", stdin);
	freopen("C-small-attempt0.out", "w", stdout);
	int T;
	cin >> T;
	for (int tt = 1 ; tt <= T ; tt++)
	{
		int n,m,k;
		cin >> n >> m >> k;
		cout << "Case #" << tt << ": ";
		if (k < 5)
		{
			cout << k << endl;
		}
		else {
			if (m <= 2 || n <= 2)
			{
				cout << k << endl;
			}
			else {
				int max = 100000;
				for (int i = 3 ; i <= n ; i++)
					for (int j = 3 ; j <= m ; j++)
						if (i * j >= k && i * j - 4 <= k)
						{
							int now = k - (i * j - 4) + 2 * (i + j - 4);
							if (now < max) 
							{
								//cout << i << " " << j << " " << " is " << now << endl;
								max = now;
							}
						}
				cout << max << endl;
			}
		}
	}
}