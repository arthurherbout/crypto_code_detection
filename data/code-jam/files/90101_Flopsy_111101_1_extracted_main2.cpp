#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

enum dir{N, W, E, S, NONE};

struct area
{
	int height;
	dir flow;
	char label;
};

char flow(int row, int col, area map[][100], char& current_label);

int main()
{
	// ifstream inf("in2.txt");
	// ifstream inf("B-small-attempt0.in");
	ifstream inf("B-large.in");
	ofstream outf("out2.txt");
	
	int t;
	int h;
	int w;
	area map[100][100];

	inf >> t;

	for (int i = 0; i < t; i++)
	{
		inf >> h >> w;

		for (int j = 0; j < h; j++)
		{
			for (int k = 0; k < w; k++)
			{
				inf >> map[j][k].height;
				map[j][k].flow = NONE;
				map[j][k].label = '?';
			}
		}

		for (int j = 0; j < h; j++)
		{
			for (int k = 0; k < w; k++)
			{
				vector<int> heights;

				heights.push_back(map[j][k].height);

				if (j > 0)
				{
					heights.push_back(map[j - 1][k].height);
				}
				if (j < h - 1)
				{
					heights.push_back(map[j + 1][k].height);
				}
				if (k > 0)
				{
					heights.push_back(map[j][k - 1].height);
				}
				if (k < w - 1)
				{
					heights.push_back(map[j][k + 1].height);
				}

				int min_height = *min_element(heights.begin(), heights.end());

				if (map[j][k].height == min_height)
				{
					map[j][k].flow = NONE;
				}
				else
				{
					if (j < h - 1)
					{
						if (map[j + 1][k].height == min_height)
						{
							map[j][k].flow = S;
						}
					}
					if (k < w - 1)
					{
						if (map[j][k + 1].height == min_height)
						{
							map[j][k].flow = E;
						}
					}
					if (k > 0)
					{
						if (map[j][k - 1].height == min_height)
						{
							map[j][k].flow = W;
						}
					}
					if (j > 0)
					{
						if (map[j - 1][k].height == min_height)
						{
							map[j][k].flow = N;
						}
					}
				}
			}
		}

		char current_label = 'a';

		for (int j = 0; j < h; j++)
		{
			for (int k = 0; k < w; k++)
			{
				if (map[j][k].label == '?')
				{
					map[j][k].label = flow(j, k, map, current_label);
				}
			}
		}

		outf << "Case #" << i + 1 << ":" << "\n";

		for (int j = 0; j < h; j++)
		{
			for (int k = 0; k < w; k++)
			{
				outf << map[j][k].label << " ";
			}

			outf << "\n";
		}
	}
}

char flow(int row, int col, area map[][100], char& current_label)
{
	switch (map[row][col].flow)
	{
	case N:
		if (map[row - 1][col].label != '?')
		{
			map[row][col].label = map[row - 1][col].label;
		}
		else
		{
			map[row][col].label = flow(row - 1, col, map, current_label);
		}
		return map[row][col].label;

	case W:
		if (map[row][col - 1].label != '?')
		{
			map[row][col].label = map[row][col - 1].label;
		}
		else
		{
			map[row][col].label = flow(row, col - 1, map, current_label);
		}
		return map[row][col].label;

	case E:
		if (map[row][col + 1].label != '?')
		{
			map[row][col].label = map[row][col + 1].label;
		}
		else
		{
			map[row][col].label = flow(row, col + 1, map, current_label);
		}
		return map[row][col].label;

	case S:
		if (map[row + 1][col].label != '?')
		{
			map[row][col].label = map[row + 1][col].label;
		}
		else
		{
			map[row][col].label = flow(row + 1, col, map, current_label);
		}
		return map[row][col].label;

	case NONE:
		if (map[row][col].label == '?')
		{
			map[row][col].label = current_label;
			current_label++;
		}
		return map[row][col].label;
	}
}
