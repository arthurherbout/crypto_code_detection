// Problem C
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

pair<int,pair<int,int> > solve(int x, int y, vector<vector<char> >& cell, vector<int>& sep_x, vector<int>& sep_y)
{
	if(!cell[x][y])
		return pair<int,pair<int,int> >(100000000,pair<int,int>(0,0));
	
	cell[x][y] = false;
	
	const int dx[] = {0,0,1,-1,1,-1};
	const int dy[] = {1,-1,0,0,-1,1};
	int first = sep_x[x]+sep_y[y], secondX = sep_x[x+1]-1, secondY = sep_y[y+1]-1;
	for(int i = 0; i < 6; i++)
	{
		pair<int,pair<int,int> > tmp = solve(x+dx[i],y+dy[i], cell, sep_x, sep_y);
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
		
		vector<int> sep_x, sep_y;
		sep_x.push_back(0);
		sep_x.push_back(1);
		sep_x.push_back(1000000);
		sep_y.push_back(0);
		sep_y.push_back(1);
		sep_y.push_back(1000000);
		vector<vector<char> > cell(3);
		cell[0] = vector<char>(3);
		cell[1] = vector<char>(3);
		cell[2] = vector<char>(3);
		
		while(R--)
		{
			int x[2], y[2];
			cin >> x[0] >> y[0] >> x[1] >> y[1];
			x[1]++;
			y[1]++;
			
			int sep_i[2], sep_j[2];
			for(int k = 0; k < 2; k++)
			{
				for(sep_i[k] = 0; sep_i[k] < sep_x.size(); sep_i[k]++)
				{
					if(sep_x[sep_i[k]] == x[k])
						break;
					if(sep_x[sep_i[k]] > x[k])
					{
						sep_x.insert(sep_x.begin() + sep_i[k], x[k]);
						cell.insert(cell.begin() + sep_i[k], cell[sep_i[k]-1]);
						break;
					}
				}
			}
			for(int k = 0; k < 2; k++)
			{
				for(sep_j[k] = 0; sep_j[k] < sep_y.size(); sep_j[k]++)
				{
					if(sep_y[sep_j[k]] == y[k])
						break;
					if(sep_y[sep_j[k]] > y[k])
					{
						sep_y.insert(sep_y.begin() + sep_j[k], y[k]);
						for(int i = 0; i < cell.size(); i++)
						cell[i].insert(cell[i].begin() + sep_j[k], cell[i][sep_j[k]-1]);
						break;
					}
				}
			}
			
			for(int i = sep_i[0]; i < sep_i[1]; i++)
				for(int j = sep_j[0]; j < sep_j[1]; j++)
					cell[i][j] = true;
		}
		
		/*
		for(int i = 0; i < sep_x.size(); i++)
		{
			cout << sep_x[i] << ",";
		}
		cout << endl;
		for(int i = 0; i < cell.size(); i++)
		{
			for(int j = 0; j < cell[i].size(); j++)
			{
				cout << (cell[i][j] ? "#" : ".");
			}
			cout << endl;
		}
		cout << endl;
		cout << endl;
		cout << endl;
		*/
		
		int ans = 0;
		for(int i = 1; i < cell.size()-1; i++)
			for(int j = 1; j < cell[i].size()-1; j++)
			{
				pair<int,pair<int,int> > tmp = solve(i,j,cell, sep_x, sep_y);
				ans = max(ans, (tmp.second.first + tmp.second.second) - tmp.first + 1);
			}
		
		printf("Case #%d: %d\n", No, ans);
	}
	return 0;
}
