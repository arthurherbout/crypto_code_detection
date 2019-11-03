#include <iostream>
#include <algorithm>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

const double eps = 1e-12;

double f[1<<20];

double Work()
{
	string s;
	int n;
	cin >> s;
	n = s.length();
	int mst = 1 << n, sst = 0;
	for (int i = 0; i < n; i ++)
		if (s[i] == '.')
			sst += 1 << i;

	memset(f, 0, sizeof(f));

	for (int v = 0; v < sst; v ++)
		if ((v | sst) == sst)
		{
			for (int i = 0; i < n; i ++)
				if (s[i] == '.' && !(v & (1 << i)))
				{
					int z = v + (1 << i), sm = 0, ss = n;
					for (int k = i; ss; k = (k + n - 1) % n)
					{
						if (v & (1 << k))  break;
						f[z] += (f[v] + (ss --)) / n;
					}
				}
		}

	return f[sst];
}

int main()
{
	freopen("D-small-attempt0.in", "r", stdin);
	freopen("D-small-output.txt", "w", stdout);
	ios::sync_with_stdio(false);

	int T;
	cin >> T;
	cout.precision(15);
	for (int tt = 1; tt <= T; tt ++)
	{
		cout << "Case #" << tt << ": ";
		cout << Work();
		cout << endl;
	}

	return 0;
}