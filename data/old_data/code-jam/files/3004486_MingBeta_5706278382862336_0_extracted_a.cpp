#include <iostream>

using namespace std;

long long gcd(long long x, long long y)
{
	if (x % y == 0) return y;
	else
		return gcd(y, x % y);
}

int main()
{

	freopen("A-small-attempt0.in", "r", stdin);
	freopen("A-small-attempt0.out", "w", stdout);
	int T;
	cin >> T;
	for (int tt = 1 ; tt <= T; tt++)
	{
		string s;
		cin >> s;
		int l = s.length();
		long long numb[2];
		numb[0] = numb[1] = 0;
		int now = 0;
		for (int i = 0 ; i < l ; i++)
		{
			if (s[i] == '/') now++;
			else
			{
				numb[now] = numb[now] * 10 + (long long)(s[i] - '0');
			}
		}

		int ans = 0;

		long long ggcd = gcd(numb[0], numb[1]);
		numb[0] /= ggcd;
		numb[1] /= ggcd;

		int now_f = numb[1];
		while (now_f != 1)
		{
			if (now_f % 2 != 0)
			{
				ans = -1;
				break;
			}
			else {
				now_f /= 2;
			}
		}

		if (ans == -1)
		{
			cout << "Case #" << tt << ": " << "impossible" << endl;
		}
		else {

			while (numb[1] != 1)
			{
				if (numb[1] > numb[0]) 
				{
					numb[1] /= 2;
					ans ++;
				}
				else break;
			}
			cout << "Case #" << tt << ": " << ans << endl;
		}
	}
}