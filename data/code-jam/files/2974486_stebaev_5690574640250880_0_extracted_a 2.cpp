#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

void go(vector<vector<int> > &pos, int I, int J, int& closed)
{
	if (pos[I][J] == 1) return;
	if (pos[I][J] == 0) return;

	int r = pos.size();
	if (r == 0) return;
	int c = pos[0].size();

	pos[I][J] = 0;
	closed--;	

	for (int i = I - 1; i <= I + 1; i++)
		for (int j = J - 1; j <= J + 1; j++)
		{
			if (i == I && j == J) continue;
			if (j >= 0 && i >= 0 && i < r && j < c)
				if (pos[i][j] == 1)
					return;
		}

	for (int i = I - 1; i <= I + 1; i++)
		for (int j = J - 1; j <= J+1; j++)
			if (j >= 0 && i >= 0 && i < r && j < c)
				go(pos, i, j, closed);
}

void solve(int r, int c, int m)
{
	vector<bool> state(r*c, false);
	for (int i = 0; i < m; i++)
		state[i] = true;	

	while (true)
	{
		/*for (int i = 0; i < state.size(); i++)
			if (state[i]) cout << '*';
			else cout << '.';
			cout << endl;*/

		//make board
		vector<vector<int> > pos(r, vector<int>(c, 2));
		for (int i = 0; i < r; i++)
			for (int j = 0; j < c; j++)
				if (state[i*c + j])
					pos[i][j] = 1;		

		//try to win
		for (int i = 0; i < r; i++)
			for (int j = 0; j < c; j++)
			{
				vector<vector<int> > _pos = pos;
				int closed = r*c;
				go(_pos, i, j, closed);

				if (closed == m)
				{
					//we found the solution
					for (int _i = 0; _i < r; _i++)
					{
						for (int _j = 0; _j < c; _j++)
						{
							if (_i == i && _j == j)
								cout << 'c';
							else if (_pos[_i][_j] == 1)
								cout << '*';
							else cout << '.';
						}
						cout << endl;
					}

					return;
				}
			}

		//next state
		int passed = 0;
		bool exists = false;
		for (int i = r*c - 1; i >= 0; i--)
		{
			if (state[i])
				passed++;

			if (i + 1 >= r*c) continue;

			if (state[i] && !state[i + 1])
			{
				exists = true;
				state[i] = false;
				for (int j = i+1; j < r*c; j++)
					if (passed)
					{
						passed--;
						state[j] = true;
					}
					else{
						state[j] = false;
					}
				break;
			}			
		}
		if (!exists)
			break;
	}

	cout << "Impossible" << endl;
}

int main(int argc, char* argv[])
{
	FILE* fin = NULL;
	FILE* fout = NULL; 

	fin = freopen("input.txt", "r", stdin);
	fout = freopen("output.txt", "w", stdout);	

	int T; cin >> T;
	for (int test = 1 ; test<= T; test++)
	{
		int r, c, m;
		cin >> r >> c >> m;

		printf("Case #%d:\n", test);
		solve(r, c, m);		
	}

	if (fin) fclose(fin);
	if (fout) fclose(fout);

	return 0;
}

