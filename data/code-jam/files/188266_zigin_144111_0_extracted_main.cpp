#include <iostream>
#include <fstream>
using namespace std;

int cross[20][20][3];

int dfs(int x, int y, bool passX, bool passY, int rowCount, int columnCount, int curTime)
{
	if (x == columnCount - 1 && y == 0 && passX && passY) {
		return curTime;
	}
	int ret = INT_MAX;
	if (passX) {
		if (x + 1 < columnCount) {
			ret = min(ret, dfs(x + 1, y, false, passY, rowCount, columnCount, curTime + 2));
		}
	} else {
		int tmp = curTime % (cross[y][x][0] + cross[y][x][1]);
		int waitTime = 0;
		if (tmp < cross[y][x][2] - cross[y][x][1]) {
			waitTime = cross[y][x][2] - cross[y][x][1] - tmp;
		}
		if (tmp >= cross[y][x][2] && tmp < cross[y][x][2] + cross[y][x][0]) {
			waitTime = cross[y][x][2] + cross[y][x][0] - tmp;
		}
		ret = min(ret, dfs(x, y, true, passY, rowCount, columnCount, curTime + 1 + waitTime));
	}

	if (passY) {
		if (y - 1 >= 0) {
			ret = min(ret, dfs(x, y - 1, passX, false, rowCount, columnCount, curTime + 2));
		}
	} else {
		int tmp = curTime % (cross[y][x][0] + cross[y][x][1]);
		int waitTime = 0;
		if (tmp >= cross[y][x][2] + cross[y][x][0]) {
			waitTime = cross[y][x][2] + cross[y][x][1] + cross[y][x][0] - tmp;
		}
		if (tmp < cross[y][x][2] && tmp >= cross[y][x][2] - cross[y][x][1]) {
			waitTime = cross[y][x][2] - tmp;
		}
		ret = min(ret, dfs(x, y, passX, true, rowCount, columnCount, curTime + 1 + waitTime));
	}
	return ret;
}

int main()
{
	fstream file("B.in");
	fstream output("B.out", ios_base::out);

	int caseIndex = 0, caseCount;
	file >> caseCount;

	while (++caseIndex <= caseCount)
	{
		int m, n;
		file >> n >> m;
		for (int i = 0; i < n; ++ i)
		{
			for (int j = 0; j < m; ++ j)
			{
				file >> cross[i][j][0] >> cross[i][j][1] >> cross[i][j][2];
				cross[i][j][2] %= cross[i][j][0] + cross[i][j][1];
			}
		}
		output << "Case #" << caseIndex << ": " << dfs(0, n - 1, false, false, n, m, 0) << endl;
	}

	file.close();
	output.close();
	return 0;
}