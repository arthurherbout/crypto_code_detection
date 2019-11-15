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
#ifdef WIN32
typedef __int64 ll;
#else
typedef long long ll;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//ofstream fout("output.out");
FILE *fout = fopen ("output.out", "w");
ifstream fin("input.in");

struct rec
{
	int x, y, z, p;
};

double count(double x, double y, double z, const vector<rec> & ships)
{
	double res = 0.0;
	FOR(i, 0, ships.size())
		res = max(res, (fabs(x-ships[i].x)+fabs(y-ships[i].y)+fabs(z-ships[i].z)) / (double)ships[i].p);
	return res;
}

int main()
{
	int T;
	fin >> T;

	FOR(CASE, 0, T)
	{
		int N;
		fin >> N;
		
		vector<rec> ships(N);
		FOR(i, 0, N)
			fin >> ships[i].x >> ships[i].y >> ships[i].z >> ships[i].p;

		double x = 0.0, y = 0.0, z = 0.0, res = count(0.0, 0.0, 0.0, ships);
		double step = 100000.0;

		while (step > 1e-50)
		{
			double act;
			if ((act = count(x+step, y, z, ships)) < res)
			{
				x += step;
				res = act;
			}
			else if ((act = count(x-step, y, z, ships)) < res)
			{
				x -= step;
				res = act;
			}
			else if ((act = count(x, y+step, z, ships)) < res)
			{
				y += step;
				res = act;
			}
			else if ((act = count(x, y-step, z, ships)) < res)
			{
				y -= step;
				res = act;
			}
			else if ((act = count(x, y, z+step, ships)) < res)
			{
				z += step;
				res = act;
			}
			else if ((act = count(x, y, z-step, ships)) < res)
			{
				z -= step;
				res = act;
			}
			else step /= 2.0;
		}

		//cout << "Case #" << CASE+1 << ": " << res << endl;
		fprintf(fout, "Case #%d: %.6lf\n", CASE+1, res);
	}

	return 0;
}
