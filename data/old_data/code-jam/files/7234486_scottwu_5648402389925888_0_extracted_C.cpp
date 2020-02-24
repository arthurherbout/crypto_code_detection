#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <cmath>

using namespace std;
typedef long long ll;
typedef long double ld;
const double PI = acos(-1.0);
const int MAXM = 1e6;

ifstream fin ("C.in");
ofstream fout ("C.out");

double hype (double x, double y) // hype
{
	return sqrt (x * x + y * y);
}

int N, M;
double R;
map <double, int> interesting;
vector <pair <double, double> > res;

bool good[MAXM];

int main()
{
	int ntest; fin >> ntest;
	for (int test = 0; test < ntest; test++)
	{
	interesting.clear();
	res.clear();
	
	fin >> N >> R;
	R /= 1e6;
	
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (i + j)
			{
				double ang;
				if (i > 0)
					ang = atan((double) j / (double) i);
				else
					ang = PI / 2;
				
				double cang = asin (R / hype (i, j));
				
				double lo = ang - cang, hi = ang + cang;
				//cout << i << " " << j << " " << lo << " " << hi << "\n";
				interesting[lo];
				interesting[hi];
				res.push_back (make_pair (lo, hi));
			}
	
	M = 0;
	for (map<double,int>::iterator it = interesting.begin(); it != interesting.end(); it++)
	{
		it->second = M++;
	}
	
	for (int i = 0; i < MAXM; i++)
		good[i] = true;
	
	int ans = 0;
	for (int i = 0; i < res.size(); i++)
	{
		bool bad = true;
		int lo = interesting[res[i].first], hi = interesting[res[i].second];
		
		for (int j = lo; j < hi; j++)
			if (good[j])
				bad = false;
		
		if (!bad)
		{
			ans++;
			//cout << (i + 1) / N << " " << (i + 1) % N << "\n";
		}
		
		for (int j = lo; j < hi; j++)
			good[j] = false;
	}
	
	fout << "Case #" << test + 1 << ": ";
	fout << ans << "\n";
	
	}
	
	return 0;
}
