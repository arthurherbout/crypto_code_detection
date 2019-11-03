#include <map>
#include <set>
#include <cmath>
#include <queue>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <ctime>
#include <fstream>
using namespace std;

typedef long long int64;
typedef vector<int> vi;



int main()
{
	int i,j;
	ifstream inf("A-small-attempt0.in");
	ofstream outf("A-small-attempt0.txt");

	int T = 0;
	inf>>T;

	int n = 0;
	int tem = 0;
	for (i = 0; i < T; i++)
	{
		vi x, y;
		int64 re = 0;
		inf>>n;
		for (j = 0; j < n; j++)
		{
			inf>>tem;
			x.push_back(tem);
		}
		for (j = 0; j < n; j++)
		{
			inf>>tem;
			y.push_back(tem);
		}
		sort(x.begin(),x.end());
		sort(y.begin(),y.end());
		for ( j = 0; j < n; j++)
		{
			re += x[j]*y[n-1-j];
		}
		outf<<"Case #"<<i+1<<": "<<re<<"\n";
	}
	return 0;
}