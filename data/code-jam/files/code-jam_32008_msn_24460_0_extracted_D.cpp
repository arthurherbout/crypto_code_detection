#include <algorithm>
#include <cstdio>
#include <cmath>
#include <numeric>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <stack>
#include <bitset>
#include <fstream>
#include <sstream>
#include <iostream>

#define fill(a, x)	memset(a, x, sizeof(a))
#define fillv(a, x) for (int i = 0; i < a.size(); i++) a[i] = x;
#define filla(a, x)	for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) a[i] = x
#define allv(a) a.begin(), a.end()
#define alla(a, n) a, a + n
#define sortv(a) sort(all(a))
#define sorta(a, n) sort(a, a + n)
#define mp make_pair

using namespace std;

const int MAX = 16;
const int dx[] = {0, 1, 0, -1, -1, 1, -1, 1};
const int dy[] = {-1, 0, 1, 0, -1, -1, 1, 1};

ifstream fin ("D.IN");
ofstream fout("D.OUT");
int NumTest, Test;
int n, m;
int Save[1 << MAX][4][4];
char a[4][4];

void Prepare(){
	fin >> NumTest;
}

void Enter(){
	fin >> n >> m;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) fin >> a[i][j];
}

void Init(){
	fill(Save, 0);
}

void Solve(){
}

bool Inboard(int x, int y){
	return 0 <= x && x < n && 0 <= y && y < m;
}

int Map(){
	int Temp = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (a[i][j] == '.') Temp = Temp * 2;
			else Temp = Temp * 2 + 1;
	return Temp;
}

bool Win(){
	int p = Map();
	int kx, ky;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (a[i][j] == 'K'){
				kx = i;
				ky = j;
				break;
			}
			
	if (Save[p][kx][ky] != 0) return (Save[p][kx][ky] == 1);
	for (int i = 0; i < 8; i++){
		int x = kx + dx[i];
		int y = ky + dy[i];
		if (!Inboard(x, y)) continue;
		if (a[x][y] == '#') continue;
		a[kx][ky] = '#';
		a[x][y] = 'K';
		bool Stop = !Win();
		a[kx][ky] = 'K';
		a[x][y] = '.';
		if (Stop){
			Save[p][kx][ky] = 1;
			return true;
		}
	}
	Save[p][kx][ky] = -1;
	return false;
}

void PrintResult(){
	fout << "Case #" << Test + 1 << ": ";
	if (Win() == true) fout << "A";
	else fout << "B";
	fout << endl;
}

void Done(){
	fout.close();
}

int main(){
	Prepare();
	for (Test = 0; Test < NumTest; Test++){		
		Enter();
		Init();
		Solve();
		PrintResult();
	}
	Done();
//	getchar();
}
