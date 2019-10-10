#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
using namespace std;

bool was[100];

double x[100];
double y[100];
double r[100];

double length(int i, int j)
{
	return sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
}

double getAn(int i, int j)
{
	return length(i, j) + r[i] + r[j];
}

int main()
{
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);

	int t;
	cin >> t;

	int b[100];

	

	for (int k=0; k<t; k++)
	{
		int n;
		cin >> n;

		double D = 0;

		for (int i=0;i<n;i++)
		{
			cin >> x[i] >> y[i] >> r[i];
			
			D = max(D, r[i] * 2.0);
		}

		double ans = 20000000;

		for (int i=0;i<n-1;i++)
		{
			for (int j=i+1;j<n;j++)
			{
				ans = min(ans, getAn(i,j));
			}
		}

		ans = max(ans, D / 2.0);

		if (n == 1)
			ans = r[0] * 2.0;

		cout.setf(ios::fixed);
		cout.precision(8);

		cout << "Case #" << k+1 << ": " << ans / 2.0 << endl;

	}

	return 0;
}