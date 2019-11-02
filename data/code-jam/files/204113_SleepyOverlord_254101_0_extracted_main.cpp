//Code by Patcas Csaba
//Time complexity:
//Space complexity:
//Method:
//Implementation time: 

#include <vector>
#include <string> 
#include <set> 
#include <map> 
#include <queue> 
#include <bitset> 

#include <numeric> 
#include <algorithm> 

#include <cstdio>
#include <fstream>
#include <iostream> 
#include <sstream> 

#include <cctype>
#include <cmath> 
#include <ctime>
#include <cassert>

using namespace std;

#define VI vector <int>
#define VVI vector <VI>
#define VS vector <string>
#define VB vector <bool>
#define PII pair <int, int>
#define LL long long

#define FORN(i, n) for(int i = 0; i < (n); ++i)
#define FOR(i, a, b) for(int i = (a); i <= (b); ++i)
#define FORI(it, X) for(__typeof((X).begin()) it = (X).begin(); it !=(X).end(); ++it) 
#define REPEAT do{ 
#define UNTIL(x) }while(!(x)); 

#define SZ size()
#define BG begin() 
#define EN end() 
#define CL clear()
#define X first
#define Y second
#define RS resize
#define PB push_back
#define MP make_pair
#define ALL(x) x.begin(), x.end()

#define BASE 1000033

#define in_file "b.in"
#define out_file "b.out"
ifstream fin(in_file);
ofstream fout(out_file);

int n, m, f;
VVI a;
bool found;
set <LL> was;

inline bool OnBoard(int x, int y)
{
	return (x >= 1 && x <= n && y >= 1 && y <= m);
}

int Back(int x, int y)
{
	//cout << x << " " << y << endl;
	LL hash = 0;
	FOR(i, 1, n)
		FOR(j, 1, m) hash = (hash * 2 + a[i][j]) % BASE;
	hash = (hash * (n + 1) + x) % BASE;
	hash = (hash * (m + 1) + y) % BASE;
	if (was.count(hash)) return n * m;
	was.insert(hash);
	
	int fall = 0;
	while (x < n && !a[x + 1][y])
	{
		++x, ++fall;
	}
	if (fall > f) return n * m;
	if (x == n)	
	{
		//found = true;
		return 0;
	}
	int ans = n * m;
	if (OnBoard(x, y + 1) && !a[x][y + 1]) 
	{
		//was[x][y] = true;
		ans = min(ans, Back(x, y + 1));
		if (found) return ans;
		//was[x][y] = false;
	}
	if (OnBoard(x, y - 1) && !a[x][y - 1]) 
	{
		//was[x][y] = true;
		ans = min(ans, Back(x, y - 1));
		if (found) return ans;
		//was[x][y] = false;
	}
	if (OnBoard(x, y + 1) && OnBoard(x + 1, y + 1) && !a[x][y + 1] && a[x + 1][y + 1]) 
	{
		a[x + 1][y + 1] = 0;
		ans = min(ans, Back(x, y) + 1);
		if (found) return ans;
		a[x + 1][y + 1] = 1;
	}
	if (OnBoard(x, y - 1) && OnBoard(x + 1, y - 1) && !a[x][y - 1] && a[x + 1][y - 1]) 
	{
		a[x + 1][y - 1] = 0;
		ans = min(ans, Back(x, y) + 1);
		if (found) return ans;
		a[x + 1][y - 1] = 1;
	}
	return ans;
}

int Solve()
{
	//was.RS(n + 1, VI(m + 1));
	was.CL;
	found = false;
	return Back(1, 1);
}

int main()
{
	//Read data
	int test;
	fin >> test;
	FORN(t, test)
	{
		cout << t << endl;
		fin >> n >> m >> f;
		a.CL, a.RS(n + 1, VI(m + 1));
		string aux;
		FOR(i, 1, n)
		{
			fin >> aux;
			FORN(j, m) a[i][j + 1] = aux[j] == '#';
		}
		int aux2 = Solve();
		if (aux2 >= n * m) fout << "Case #" << t + 1 << ": " << "No" << endl;
		else fout << "Case #" << t + 1 << ": " << "Yes " << aux2 << endl;
	}
	fin.close();

	//Solve

	//Write data
	fout.close();

	return 0;
}