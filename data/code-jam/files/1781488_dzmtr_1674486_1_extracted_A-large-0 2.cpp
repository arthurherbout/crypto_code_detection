#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int T;
	cin >> T;

	for (int t = 1; t <= T; ++t)
	{
		int n;
		cin >> n;
		vector< vector<int> > g(n);
		for (int i = 0; i < n; ++i)
		{
			int m;
			cin >> m;
			for (int j = 0; j < m; ++j)
			{
				int x;
				cin >> x;
				g[i].push_back(x - 1);
			}
		}

		bool found = false;
		for (int i = 0; i < n; ++i)
		{
			vector<int> v(n);
			queue<int> q;
			q.push(i);
			v[i] = 1;
			while (!q.empty())
			{
				int x = q.front();
				q.pop();
				for (vector<int>::const_iterator y = g[x].begin(); y != g[x].end(); ++y)
					if (v[*y] == 1)
					{
						found = true;
						goto end;
					}
					else
					{
						v[*y] = 1;
						q.push(*y);
					}
			}
		}
end:
		if (found)
			cout << "Case #" << t << ": Yes\n";
		else
			cout << "Case #" << t << ": No\n";

	}
}
