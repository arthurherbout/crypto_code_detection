#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cctype>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
using namespace std; 

#define DEBUG(x) cerr << '>' << #x << ':' << x << endl;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define FOR2(i, a, b) for (int i = (a); i > (b); --i)
inline bool EQ(double a, double b) { return fabs(a-b) < 1e-9; }
const int INF = 1 << 29;

inline int two(int n) { return 1 << n; }
inline int test(int n, int b) { return n & two(b); }
inline void set_bit(int & n, int b) { n |= two(b); }
inline void unset_bit(int & n, int b) { n &= ~two(b); }
inline int last_bit(int n) { return n & (-n); }
inline int ones(int n)
{
	int res = 0;
	while (n) { ++res; n -= last_bit(n); }
	return res;
}
#ifdef WIN32
typedef __int64 ll;
#else
typedef long long ll;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

ifstream fin("input.in");
ofstream fout("output.out");
//FILE *fin  = fopen ("input.in", "r");
//FILE *fout = fopen ("output.out", "w");

struct rec 
{ 
	int begin, end, col; 
	bool operator<(const rec & r) const { return begin < r.begin; };
} paints[347];

int main()
{
	int T;
	fin >> T;

	FOR(step, 0, T)
	{
		int N;
		fin >> N;

		string temp;
		map<string, int> colors;

		int num_cols = 0;
		FOR(i, 0, N)
		{
			fin >> temp >> paints[i].begin >> paints[i].end;
			if (colors.find(temp) == colors.end())
				colors[temp] = num_cols++;
			paints[i].col = colors[temp];
		}
		sort(paints, paints+N);

		int res = INF;

		FOR(col1, 0, N)
			FOR(col2, col1, N)
				FOR(col3, col2, N)
				{
					int act = 0, last = 0, next = 0;
					FOR(i, 0, N)
					{
						if (last + 1 < paints[i].begin)
							if(next+1 < paints[i].begin)
							{
								act = INF;
								break;
							}
							else
							{
								last = next;
								++act;
							}
						if (paints[i].col != col1 && paints[i].col != col2 && paints[i].col != col3)
							continue;
						
						next = max(next, paints[i].end);
					}
					if (last < 10000)
					{
						if (next < 10000) act = INF;
						else ++act;
					}
					res = min(res, act);
				}
		fout << "Case #" << step+1 << ": ";
		if (res == INF)
			fout << "IMPOSSIBLE";
		else
			fout << res;
		fout << endl;
	}

	return 0;
}
