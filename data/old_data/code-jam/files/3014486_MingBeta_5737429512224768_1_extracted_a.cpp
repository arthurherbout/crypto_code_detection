#include <iostream>

using namespace std;

int main()
{
	freopen("A-large.in", "r", stdin);
	freopen("A-large.out", "w", stdout);
	int T;
	cin >> T;
	for (int tt = 1 ; tt <= T ; tt++)
	{
		int n, s;
		cin >> n >> s;
		int nums[701];
		memset(nums, 0, sizeof(nums));
		for (int i = 0 ; i < n ; i++)
		{
			int x;
			cin >> x;
			nums[x] ++;
		}

		int ans = 0;

		for (int i = 700 ; i > 0 ; i--)
		{
			if (nums[i] == 0) continue;
			int now = nums[i];
			nums[i] = 0;
			for (int j = s - i ; j > 0 ; j--)
			{
				if (now == 0) break;
				if (nums[j] >= now)
				{
					ans += now;
					nums[j] -= now;
					now = 0;
				}
				else if (nums[j] < now)
				{
					ans += nums[j];
					now -= nums[j];
					nums[j] = 0;
				}
			}
			//cout << i << " mad " << ans << endl;
			if (now > 0) 
			{
				if (i <= s / 2) ans += (now + 1) / 2;
				else ans += now;
			}
		}

		cout << "Case #" << tt << ": " << ans << endl;

	}
}