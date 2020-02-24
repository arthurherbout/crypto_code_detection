// Problem C
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

pair<int,pair<int,int> > solve(int x, int y, bool cell[102][102])
{
	if(!cell[x][y])
		return pair<int,pair<int,int> >(200,pair<int,int>(0,0));
	
	cell[x][y] = false;
	
	const int dx[] = {0,0,1,-1,1,-1};
	const int dy[] = {1,-1,0,0,-1,1};
	int first = x+y, secondX = x, secondY = y;
	for(int i = 0; i < 6; i++)
	{
		pair<int,pair<int,int> > tmp = solve(x+dx[i],y+dy[i], cell);
		first = min(first, tmp.first);
		secondX = max(secondX, tmp.second.first);
		secondY = max(secondY, tmp.second.second);
	}
	
	return pair<int,pair<int,int> >(first, pair<int,int>(secondX, secondY));
}

int main()
{
	int C;
	cin >> C;
	for(int No = 1; No <= C; No++)
	{
		int R;
		cin >> R;
		
		bool cell[102][102] = {};
		
		while(R--)
		{
			int x[2], y[2];
			cin >> x[0] >> y[0] >> x[1] >> y[1];
			
			for(int i = x[0]; i <= x[1]; i++)
				for(int j = y[0]; j <= y[1]; j++)
					cell[i][j] = true;
		}
		
		int ans = 0;
		for(int i = 1; i <= 100; i++)
			for(int j = 1; j <= 100; j++)
			{
				pair<int,pair<int,int> > tmp = solve(i,j,cell);
				ans = max(ans, (tmp.second.first + tmp.second.second) - tmp.first + 1);
			}
		
		printf("Case #%d: %d\n", No, ans);
	}
	return 0;
}
