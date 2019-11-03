#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
using namespace std;

bool was[100];

int main()
{
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);

	int t;
	cin >> t;

	int b[100];

	for (int k=0; k<t; k++)
	{
		int n;
		cin >> n;
		for (int i=0; i<n; i++)
		{
			string s;
			cin >> s;
			b[i] = 1000000;
			int j = n - 1;

			while (s[j]=='0' && j>0) j--;

			b[i] = j;

		}

	int ans = 0;

	for (int i = 0; i < n; ++i)
	{
		for (int j = i; j < n; ++j)
		{
			if (was[j]) continue;
			if (b[j]<=i)
			{
				for (int kk = j; i < kk; kk--)
				{
					int t = b[kk];
					b[kk] = b[kk - 1];
					b[kk - 1] = t;
					ans++;
				}
				break;
			}
		}
	}

	cout << "Case #" << k + 1 << ": " << ans << endl;

	}

	return 0;
}