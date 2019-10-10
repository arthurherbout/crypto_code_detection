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
#include <queue>
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

int m,n;
int mark[60][60];
bool check[60][60];
int ans;

int dx[4] = {0,1,0,-1};
int dy[4] = {1,0,-1,0};

int q[2600];
bool qm[2600];
int r,w;

int neigh(int x, int y, int t)
{
	int ret = 0;
	FI(d,0,4)
	{
		int x1 = x+dx[d];
		int y1 = y+dy[d];
		if (mark[x1][y1] == t) ret++;
	}
	return ret;
}

void checknode(int i, int j)
{
	if (!check[i][j]) return;
	int c1 = neigh(i,j,1);
	if (c1 >= 2)
	{
		check[i][j] = false;
		qm[w] = false;
		q[w++] = (i-1)*n+j-1;
	}
	else if (c1 == 0)
	{
		int c2 = neigh(i,j,-1);
		if (c2 <= 1)
		{
			check[i][j] = false;
			qm[w] = true;
			q[w++] = (i-1)*n+j-1;
		}
	}
}

int sum[2];
void dfs(int x, int y, int s)
{
	mark[x][y] = 2;
	int c = neigh(x,y,1);
	sum[s] += 4-c*2;
	FI(d,0,4)
	{
		int x1 = x+dx[d];
		int y1 = y+dy[d];
		if (mark[x1][y1] == -1)
			dfs(x1,y1,1-s);
	}
}

int a[1<<15];
int b[1<<15];

int main()
{
	int mask[20] = {};
	int mask2[20] = {};
	int tcase = 0;
	ifstream fin("z.in");
	ofstream fout("z.out");
	fin >> tcase;
	for (int tind = 1; tind <= tcase; tind++)
	{
		fin >> m >> n;
		mset(mark, 0);
		FI(i,0,m)
		{
			string str;
			fin >> str;
			FI(j,0,n)
				if (str[j] == '#') mark[i+1][j+1] = 1;
				else if (str[j] == '?') mark[i+1][j+1] = -1;
		}
		

		int zz = 1<<n;
		FI(i,1,n+1)
			mask[i] = zz-1-(1<<(i-1));
		FI(i,1,n+1)
			mask2[i] = 1<<(i-1);
		mset(a, 255);
		a[0] = 0;
		FI(i,1,m+1)
			FI(j,1,n+1)
		{
			mset(b, 255);
			FI(k,0,zz) if (a[k] >= 0)
			{
				if (mark[i][j] != 1)
				{
					int k1 = k & mask[j];
					if (b[k1] < a[k]) b[k1] = a[k];
				}
				if (mark[i][j] != 0)
				{
					int v = 4+a[k];
					if (k & mask2[j]) v -= 2;
					if (k & mask2[j-1]) v -= 2;
					int k1 = k | mask2[j];
					if (b[k1] < v) b[k1] = v;
				}
			}
			mcpy(a, b);
		}
		
		ans = 0;
		FI(i,0,zz)
			if (a[i] > ans) ans = a[i];
		fout << "Case #" << tind << ": " << ans << endl;
	}
	return 0;
}
