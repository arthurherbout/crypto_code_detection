#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
//#include <utility>
//#include <set>
//#include <map>
//#include <queue>
#include <iomanip>
using namespace std;

#define mset(A,B) memset(A,B,sizeof(A));
#define mcpy(A,B) memcpy(A,B,sizeof(B));
typedef long long ll;
typedef long double ld;
typedef vector<int> vint;
//typedef vector<string> vstr;
#define FI(I,L,U) for (int I=L;I<U;I++)
#define sqr(x) ((x)*(x))

int r,c;
int a[50][50];
int b[50][50];
int d[50][50];
int upa[50][50];
int upb[50][50];

void step1(int (*a)[50], int (*b)[50])
{
	int r1 = r/3;
	int c1 = c/3;
		int i = 0;
		for (int j = 0; j < c; j++)
			b[i+2][j] = a[i+1][j]-a[i][j];
	FI(i1,1,r1)
	{
		int i = i1*3;
		for (int j = 0; j < c; j++)
			b[i+2][j] = a[i+1][j]-a[i][j]-b[i-1][j];
	}
	FI(i1,0,r1)
	{
		int i = i1*3;
		FI(j,0,c)
			b[i][j] = a[i+1][j]-b[i+2][j];
	}
}

int getans(int z[50])
{
	int ret = 0;
	if (c % 3 == 0)
	{
		FI(i,0,c/3)
			ret += z[i*3+1];
	}
	else if (c % 3 == 2)
	{
		FI(i,0,c/3+1)
			ret += z[i*3+1];
	}
	else
	{
		FI(i,0,c/3+1)
			ret += z[i*3];
	}
	return ret;
}

int main()
{
	int tcase = 0;
	ifstream fin("z.in");
	ofstream fout("z.out");
	fin >> tcase;
	for (int tind = 1; tind <= tcase; tind++)
	{
		fin >> r >> c;
		FI(i,0,r) FI(j,0,c) fin >> a[i][j];
		FI(i,0,r) FI(j,0,c) upa[i][j] = a[r-1-i][j];
		
		step1(a,b);
		step1(upa, upb);
		int mid = r/2;
		int ans = 0;
		if (mid % 3 == 2)
		{
			ans = getans(b[mid]);
		}
		else
		{
			mcpy(d, b);
			if (r % 3 == 0)
			{
				FI(j,0,c)
				{
					b[0][j] = upb[r-1][j];
					b[1][j] = d[0][j]-b[0][j];
				}
			}
			else	// == 1
			{
				FI(j,0,c)
				{
					b[1][j] = upb[r-2][j];
					b[0][j] = d[0][j]-b[1][j];
				}
			}
			int r1 = r/3;
			FI(i,3,mid+1)
			{
				FI(j,0,c)
					b[i][j] = a[i-1][j]-b[i-1][j]-b[i-2][j];
			}
			ans = getans(b[mid]);
		}

		fout << "Case #" << tind << ": " << ans << endl;
	}
	return 0;
}
