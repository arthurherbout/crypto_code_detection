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

const int MAX = 30;

ifstream fin ("C.IN");
ofstream fout("C.OUT");
int NumTest, Test;
int n, m;
double P[MAX][4];
int Now[MAX];
vector<double> List;
double Result;

void Prepare(){
	fin >> NumTest;
}

void Enter(){
	fin >> m >> n;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < 4; j++) fin >> P[i][j];
}

void Init(){
	List.clear();
}

void Try(int i){
	if (i == n){
		double Temp = 1;
		for (int j = 0; j < n; j++) Temp *= P[j][Now[j]];
		List.push_back(Temp);
		return;
	}
	for (int j = 0; j < 4; j++){
		Now[i] = j;
		Try(i + 1);
	}
}

void Solve(){
	Try(0);
	sort(allv(List));
	Result = 0;
	for (int i = List.size() - 1; i >= 0; i--){		
		Result += List[i];
		m--;
		if (m == 0) break;
	}	
}

void PrintResult(){
	fout << "Case #" << Test + 1 << ": " << Result << endl;
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
