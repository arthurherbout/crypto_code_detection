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

bool v[35][35];
int mark[35][35];
int r,c;

int z;
int dis[2][35][35];

int dx[4] = {0,1,0,-1};
int dy[4] = {1,0,-1,0};
void dfs(int x, int y, int foo)
{
	int qr = 0, qw = 0;
	int qx[1000], qy[1000];
	qx[qw] = x; qy[qw] = y;
	qw++;
	dis[z][x][y] = 0;
	v[x][y] = true;

	while (qr < qw)
	{
		x = qx[qr];
		y = qy[qr];
		qr++;
		FI(d,0,4)
		{
			int x1 = x+dx[d];
			int y1 = y+dy[d];
			if (mark[x1][y1] == 1 && !v[x1][y1])
			{
				dis[z][x1][y1] = dis[z][x][y]+1;
				v[x1][y1] = true;
				qx[qw] = x1;
				qy[qw] = y1;
				qw++;
			}
		}
	}
}


int main()
{
	int tcase = 0;
	ifstream fin("z.in");
	ofstream fout("z.out");
	fin >> tcase;
	for (int tind = 1; tind <= tcase; tind++)
	{
		//istringstream strin();
		mset(mark, 0);
		fin >> r >> c;
		int tx = -1, ty;
		FI(i,0,r)
		{
			string str;
			fin >> str;
			FI(j,0,c) 
				if (str[j] != '.') 
				{
					mark[i+1][j+1] = 1;
					if (i+j > 0 && str[j] == 'T')
					{
						tx = i+1; ty = j+1;
					}
				}
		}

		mset(v, 0);
		z = 0;
		dfs(1,1,0);
		int ans = 0;
		if (tx >= 0)
		{
			mset(v, 0);
			z = 1;
			dfs(tx, ty, 0);
			FI(i,1,r+1) FI(j,1,c+1) if (mark[i][j] == 1) ans += min(dis[0][i][j], dis[1][i][j]);
			cout << ans << endl;
			int q = dis[0][tx][ty];
			for (int i = 0; i <= q; i++)
				ans += i-min(i,q-i);
		}
		else
		{
			FI(i,1,r+1) FI(j,1,c+1) if (mark[i][j] == 1) ans += dis[0][i][j];
		}
		
		fout << "Case #" << tind << ": " << ans << endl;
	}
	return 0;
}
