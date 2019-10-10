#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
int value[20];
int f[20][20];
int solve(int e, int r, int n)
{
	int temp;
	memset(f, 0, sizeof(f));
	for (int i=0; i<=n; i++)
		for (int j=0; j<=e; j++)
		{
			f[i][j] = -1;
		}
	f[0][e] = 0;
	for (int i=1; i<=n; i++)
		for (int j=r; j<=e; j++)
			for (int k=0; k<=e; k++)
			{
				if ((j-r)<0) continue;
				if (j-r+k>e) continue;
				if (f[i-1][j-r+k] == -1) continue;
				temp = f[i-1][j-r+k]+value[i-1]*k;
				if (temp > f[i][j]) f[i][j] = temp;
			}
	temp = 0;
	for (int i=0; i<=e;i++)
	{
		if (f[n][i]>temp) temp = f[n][i];
	}
	return temp;
}

int main()
{
	ifstream fin("input.txt");
	ofstream fout("output.txt");
	int  t;
	fin >> t;
	int e,r,n;
	for (int i=0; i<t; i++)
	{
		fin >> e >> r >> n;
		for (int j=0; j<n; j++)
		{
			fin >> value[j];
		}
		if (r>e) r= e;
		fout << "Case #" << i+1 << ": ";
		fout << solve(e, r, n) << endl;
	}
	fin.close();
	fout.close();
	return 0;
}

