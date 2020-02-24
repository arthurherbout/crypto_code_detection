#include <iostream>
#include <queue>

using namespace std;

int main()
{
	int t;
	int r;
	int k;
	int n;
	int room;
	int revenue;

	cin >> t;

	for (int i = 0; i < t; i++)
	{
		cin >> r >> k >> n;

		queue<int> waiters;

		for (int j = 0; j < n; j++)
		{
			int g;

			cin >> g;

			waiters.push(g);
		}

		revenue = 0;

		for (int j = 0; j < r; j++)
		{
			room = k;
			queue<int> riders;

			while (!waiters.empty() && (room >= waiters.front()))
			{
				riders.push(waiters.front());
				room -= waiters.front();

				waiters.pop();
			}

			while (!riders.empty())
			{
				waiters.push(riders.front());
				revenue += riders.front();

				riders.pop();
			}
		}

		cout << "Case #" << i + 1 << ": " << revenue << "\n";
	}
}

