#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

#include<stdio.h>

vector<string> rotate(vector<string> board);
int check_rows(vector<string> board, int k);
int check_cols(vector<string> board, int k);
int check_diag(vector<string> board, int k);
int check_winner(vector<string> board, int k);

int main()
{
	int n;
	cin >> n;

	for(int i = 1; i <= n; i++)
	{
		int n, k;
		cin >> n >> k;

		vector<string> board = vector<string>(n);

		int j = 0;
		for(j = 0; j < n; j++)
			cin >> board[j];

		board = rotate(board);

		//for(int a = 0; a < board.size(); a++) cout << board[a] << endl;

		int winner = check_winner(board, k);

		cout << "Case #" << i << ": ";

		if(winner == 1) cout << "Red\n";
		else if(winner == 2) cout << "Blue\n";
		else if(winner == -1) cout << "Neither\n";
		else cout << "Both\n";
	}
}

vector<string> rotate(vector<string> board)
{
	vector<string> res;

	string str = "";

	int i;
	for(i = 0; i < board.size(); i++)
	{
		for(int j = board.size()-1; j >= 0; j--)
			str += board[j][i];

		res.push_back(str);
		str = "";
	}

	for(int col = 0; col < res.size(); col++)
	{
		for(int row = res.size()-1; row >= 0; row--)
		{
			int newRow = row+1;
			char temp = res[row][col];

			while(newRow < res.size() && res[newRow][col] == '.')
			{
				res[newRow][col] = temp;
				if(newRow == row+1)
					res[row][col] = '.';
				else res[newRow-1][col] = '.';
				newRow++;
			}
		}
	}

	return res;
}

int check_winner(vector<string> board, int k)
{
	int player[2] = {0, 0};
	int rows = check_rows(board, k);

	if(rows == 1) player[0]++;
	else if(rows == 2) player[1]++;
	else if(rows == 0) {
		player[0]++;
		player[1]++;
	}

	//cout << rows << endl;

	int cols = check_cols(board, k);

	if(cols == 1) player[0]++;
	else if(cols == 2) player[1]++;
	else if(cols == 0) {
		player[0]++;
		player[1]++;
	}

	//cout << cols << endl;

	int diag = check_diag(board, k);

	if(diag == 1) player[0]++;
	else if(diag == 2) player[1]++;
	else if(diag == 0) {
		player[0]++;
		player[1]++;
	}

	//cout << diag << endl;

	if(player[0] == player[1] && player[0] == 0) return -1;
	else if(player[0] > 0 && player[1] > 0) return 0;
	else if(player[0] > 0) return 1;
	else if(player[1] > 0) return 2;
}

int check_rows(vector<string> board, int k)
{
	int player[2] = {0, 0};

	int mem[2] = {0, 0};

	for(int i = 0; i < board.size(); i++)
	{
		mem[0] = mem[1] = 0;
		for(int j = 0; j < board.size(); j++)
		{
			if(board[i][j] == 'R') {
				mem[0]++;
				mem[1] = 0;
			}
			else if(board[i][j] == 'B') {
				mem[1]++;
				mem[0] = 0;
			}
			else
				mem[0] = mem[1] = 0;

			if(mem[0] == k) player[0]++;
			if(mem[1] == k) player[1]++;
		}
	}

	if(player[0] == player[1] && player[0] == 0) return -1;
	else if(player[0] > 0 && player[1] > 0) return 0;
	else if(player[0] > 0) return 1;
	else if(player[1] > 0) return 2;
}

int check_cols(vector<string> board, int k)
{
	int player[2] = {0, 0};

	int mem[2] = {0, 0};

	for(int i = 0; i < board.size(); i++)
	{
		mem[0] = mem[1] = 0;
		for(int j = 0; j < board.size(); j++)
		{
			if(board[j][i] == 'R') {
				mem[0]++;
				mem[1] = 0;
			}
			else if(board[j][i] == 'B') {
				mem[1]++;
				mem[0] = 0;
			}
			else {
				mem[0] = mem[1] = 0;
			}

			if(mem[0] == k) player[0]++;
			if(mem[1] == k) player[1]++;
		}
	}

	if(player[0] == player[1] && player[0] == 0) return -1;
	else if(player[0] > 0 && player[1] > 0) return 0;
	else if(player[0] > 0) return 1;
	else if(player[1] > 0) return 2;
}

int check_diag(vector<string> board, int k)
{
	int player[] = {0, 0};
	int mem[] = {0, 0};

	for(int i = 0; i < board.size(); i++)
	{
		for(int j = 0; j < board[0].size(); j++)
		{
			mem[0] = mem[1] = 0;
			if(board[i][j] == 'R') mem[0]++;
			else if(board[i][j] == 'B') mem[1]++;

			int newI  = i+1;
			int newJ = j+1;

			while(newI < board.size() && newJ < board[0].size())
			{	
				if(board[newI][newJ] == 'R') {
					mem[0]++;
					mem[1] = 0;
				}
				else if(board[newI][newJ] == 'B') {
					mem[1]++;
					mem[0] = 0;
				}
				else mem[0] = mem[1] = 0;

				if(mem[0] == k) player[0]++;
				else if(mem[1] == k) player[1]++;

				newI++;
				newJ++;
			}

			mem[0] = mem[1] = 0;
			if(board[i][j] == 'R') mem[0]++;
			else if(board[i][j] == 'B') mem[1]++;

			newI = i+1;
			newJ = j-1;

			while(newI < board.size() && newJ >= 0)
			{		
				if(board[newI][newJ] == 'R') {
					mem[0]++;
					mem[1] = 0;
				}
				else if(board[newI][newJ] == 'B') {
					mem[1]++;
					mem[0] = 0;
				}
				else mem[0] = mem[1] = 0;

				if(mem[0] == k) player[0]++;
				else if(mem[1] == k) player[1]++;

				newI++;
				newJ--;
			}
		}
	}

	if(player[0] == player[1] && player[0] == 0) return -1;
	else if(player[0] > 0 && player[1] > 0) return 0;
	else if(player[0] > 0) return 1;
	else if(player[1] > 0) return 2;
}





















