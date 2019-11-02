#include <cstdio>
#include <utility>
#include <vector>

using namespace std;

int main()
{
	int t;
	int n;
	vector< pair<int, int> > wires;
	int y;

	scanf("%d", &t);

	for (int i = 0; i < t; i++)
	{
		scanf("%d", &n);

		wires.clear();

		for (int j = 0; j < n; j++)
		{
			pair<int, int> w;

			scanf("%d %d", &w.first, &w.second);

			wires.push_back(w);
		}

		y = 0;

		for (int j = 0; j < wires.size(); j++)
		{
			for (int k = j + 1; k < wires.size(); k++)
			{
				if (((wires.at(j).first < wires.at(k).first) && (wires.at(j).second > wires.at(k).second)) ||
					((wires.at(j).first > wires.at(k).first) && (wires.at(j).second < wires.at(k).second)))
				{
					y++;
				}
			}
		}

		printf("Case #%d: %d\n", i + 1, y);
	}
}

