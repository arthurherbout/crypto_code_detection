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

#define eps 1e-6
#define M_PI 3.141592653589793
#define bs 1000000007
#define bsize 350

using namespace std;

const int INF = 1e9;
const int N = 1031;

using namespace std;

int tests, ts, n, S;
int x[N], y[N], z[N], vx[N], vy[N], vz[N];
double closest_moment[N][N];
double L[N][N], R[N][N];
double tx[N], ty[N], tz[N];

void get_pos(int id, double t)
{
	tx[id] = x[id] + vx[id] * t;
	ty[id] = y[id] + vy[id] * t;
	tz[id] = z[id] + vz[id] * t;
}

double dist_pos(int a, int b)
{
	return sqrt((tx[a] - tx[b])*(tx[a] - tx[b]) + (ty[a] - ty[b])*(ty[a] - ty[b]) + (tz[a] - tz[b])*(tz[a] - tz[b]));
}

double get_dist(int a, int b, double moment)
{
	get_pos(a,moment);
	get_pos(b,moment);
	return dist_pos(a, b);
}
double G(double a, double b)
{
	return sqrt(a*a + b*b);
}

double get_closest(int a, int b)
{
	double l, r;
	l = 0;
	r = 1e9;
	for (int iter = 1; iter <= 74; iter++)
	{
		double mid1 = l * 2 + r;
		double mid2 = l + r * 2;
		mid1 /= 3;
		mid2 /= 3;
		double dist1 = get_dist(a, b, mid1);
		double dist2 = get_dist(a, b, mid2);
		if (dist1 < dist2)
			r = mid2;
		else
			l = mid1;
	}
	return l;
}

int used[N];
int in_queue[N];
double l_time[N], r_time[N];

pair<double, double> solver(int v1, int v2)
{
	if (r_time[v1] + S<L[v1][v2] - eps || l_time[v1]>R[v1][v2])
		return make_pair(1e9, -1e9);

	double t_l = max(L[v1][v2], l_time[v1]);
	double t_r = R[v1][v2];
	return make_pair(t_l, t_r);
}

int I;

bool solve(double thold)
{
	for (int i = 0; i < n; i++)
		used[i] = 0;
	
	for (int i = 0; i < n; i++)
	{
		for (int j = i; j < n; j++)
		{
			if (get_dist(i, j, closest_moment[i][j])>thold)
			{
				L[i][j] = 1e9;
				R[i][j] = -1e9;
				continue;
			}
			double l, r;
			l = 0;
			r = closest_moment[i][j];
			for (int iter = 1; iter <= 45; iter++)
			{
				double mid = l + r;
				mid /= 2;
				if (get_dist(i, j, mid) <= thold)
					r = mid;
				else
					l = mid;
			}
			L[i][j] = l;
			l = closest_moment[i][j];
			r = 1e9;
			for (int iter = 1; iter <=45; iter++)
			{
				double mid = l + r;
				mid /= 2;
				if (get_dist(i, j, mid) <= thold)
					l = mid;
				else
					r = mid;
			}
			R[i][j] = r;
			//cout << i << "%" << j << " " << L[i][j] << " " << R[i][j] << endl;
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			L[i][j] = L[j][i];
			R[i][j] = R[j][i];
		}
	}
	for (int i = 0; i < n; i++)
	{
		l_time[i] = 1e9;
		r_time[i] = -1e9;
		in_queue[i] = 0;
	}
	l_time[0] = 0;
	r_time[0] = 0;
	queue<int> qu;
	qu.push(0);
	in_queue[0] = 1;

	//cout << "@" << endl;
	while (qu.size())
	{
		if (l_time[1] < 1e7)
			return true;
		int v = qu.front();
		//if (I==18)
		//	cout << v << " " << l_time[v] << " " << r_time[v] << endl;
		qu.pop();
		in_queue[v] = 0;

		for (int i = 0; i < n; i++)
		{
			if (i == v)
				continue;
			pair<double, double> P = solver(v, i);
			if (P.first<l_time[i] - eps || P.second>r_time[i] + eps)
			{
				l_time[i] = min(l_time[i],P.first);
				r_time[i] = max(r_time[i],P.second);
				if (in_queue[i] == 0)
				{
					in_queue[i] = 1;
					qu.push(i);
				}
			}
		}
	}
	//cout << "#" << l_time[1] << endl;
	return (l_time[1] <= 1e8);
}

int main(){
	//freopen("fabro.in","r",stdin);
	//freopen("fabro.out","w",stdout);
	freopen("F:/in.in", "r", stdin);
	freopen("F:/output.txt", "w", stdout);
	//ios_base::sync_with_stdio(0);
	//cin.tie(0);
	
	cin >> tests;
	for (; tests; --tests)
	{
		++ts;
		cerr << ts << endl;
		//n = 1000;
		//S = 1000;
		cin >> n >> S;
		for (int i = 0; i < n; i++)
		{
			x[i] = rand() % 100;
			y[i] = rand() % 100;
			z[i] = rand() % 100;
			vx[i] = rand() % 100;
			vy[i] = rand() % 100;
			vz[i] = rand() % 100;
			cin >> x[i] >> y[i] >> z[i] >> vx[i] >> vy[i] >> vz[i];
		}

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				closest_moment[i][j] = get_closest(i, j);
				//cout << i << " " << j << " " << closest_moment[i][j] << endl;
			}
		}

		double l, r;
		l = 0;
		r = 31000000;
		//l = 1;
		//r = 1;

		for (int iter = 1; iter <52;iter++)
		{
			double mid = l + r;
			mid /= 2;
			if (solve(mid))
				r = mid;
			else
				l = mid; 
			cerr << iter << endl;
			I = iter;
		}
		cout.precision(9);
		cout << "Case #" << ts << ": " << fixed<<l << endl;
	}

	cin.get(); cin.get();
	return 0;
}