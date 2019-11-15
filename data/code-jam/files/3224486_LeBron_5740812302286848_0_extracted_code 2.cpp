/*
*/

#pragma comment(linker, "/STACK:16777216")
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <string>
#include <complex>
#include <math.h>
#include <set>
#include <vector>
#include <map>
#include <queue>
#include <stdio.h>
#include <stack>
#include <algorithm>
#include <list>
#include <ctime>
#include <memory.h>
#include <assert.h>

#define y0 sdkfaslhagaklsldk
#define y1 aasdfasdfasdf
#define yn askfhwqriuperikldjk
#define j1 assdgsdgasghsf
#define tm sdfjahlfasfh
#define lr asgasgash
#define norm asdfasdgasdgsd

#define eps 1e-9
#define M_PI 3.141592653589793
#define bs 1000000007
#define bsize 350

using namespace std;

const int INF = 1e9;
const int N = 515000;

using namespace std;

int tests;
int n, s;
int x[N], y[N], z[N], vx[N], vy[N], vz[N];

int used[N];
int ts;

double gd(int a, int b)
{
	double D = (x[a] - x[b])*(x[a] - x[b]) + (y[a] - y[b])*(y[a] - y[b]) + (z[a] - z[b])*(z[a] - z[b]);
	return sqrt(D);
}

void dfs(int v, double val)
{
	used[v] = 1;
	for (int i = 0; i < n; i++)
	{
		if (used[i])
			continue;
		if (gd(i, v) <= val)
			dfs(i,val);
	}
}
bool solve(double val)
{
	for (int i = 0; i < n; i++)
	{
		used[i] = 0;
	}
	dfs(0, val);
	return used[1];
}

int main(){
	//freopen("fabro.in","r",stdin);
	//freopen("fabro.out","w",stdout);
	freopen("F:/in.txt", "r", stdin);
	freopen("F:/output.txt", "w", stdout);
	//ios_base::sync_with_stdio(0);
	//cin.tie(0);

	cin >> tests;
	for (; tests; --tests)
	{
		cin >> n >> s;
		for (int i = 0; i < n; i++)
		{
			cin >> x[i] >> y[i] >> z[i] >> vx[i] >> vy[i] >> vz[i];
		}
		double l, r;
		l = 0;
		r = 1e6;
		for (int iter = 1; iter <= 150; iter++)
		{
			double mid = l + r;
			mid /= 2;
			if (solve(mid))
				r = mid;
			else
				l = mid;
		}
		++ts;
		cout.precision(9);
		cout << "Case #" << ts << ": " << l << endl;
	}

	cin.get(); cin.get();
	return 0;
}