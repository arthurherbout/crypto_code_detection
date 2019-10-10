#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <deque>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <iomanip>
#include <cstdio>
#include <fstream>
#include <sstream>

#define For(i,a,n) for(int i =a ; i < n ; ++i )
#define all(x) (x).begin(),(x).end()
#define n(x) (int)(x).size()
#define pb(x) push_back(x)
#define fr first
#define sc second
#define fr first
#define sc second

using namespace std;
const int maxn = 110;
const int inf = 1000*1000*1000;
int t;
typedef pair<int,int> pii;
typedef pair<int,pii> pip;
bool mark[maxn][maxn];
bool mark2[maxn][maxn];
int c[maxn][maxn];
int d[maxn][maxn];
int f[maxn][maxn];
int n ,m , h;
priority_queue <pip,vector<pip>,greater<pip> > qu;

void add(int t , int i1, int j1 , int i2 , int j2 )
{
	if(c[i1][j1]-f[i2][j2] < 50)
		return;
	if(c[i2][j2]-f[i1][j1] < 50)
		return;
	if(c[i2][j2]-f[i2][j2] < 50)
		return;
	int timetogo = max(t,h-(c[i2][j2]-50) );
	int curh = h-timetogo;
	int dist ;
	if(curh - f[i1][j1] >= 20 )
		dist = 10;
	else
		dist = 100;
	qu.push(pip(timetogo+dist,pii(i2,j2)));
	return;
}
void add2(int,int,int,int);
void dfs(int, int);
void add2(int i1, int j1 , int i2 , int j2 )
{
	if(mark2[i2][j2])
		return;
//	cerr << i1 << " " << j1 << " " << i2 << " " << j2 << " " << c[i1][j1] << " "<< f[i2][j2] << endl;
	if(c[i1][j1]-f[i2][j2] < 50)
		return;
	if(c[i2][j2]-f[i1][j1] < 50)
		return;
	if(c[i2][j2]-f[i2][j2] < 50)
		return;
	int timetogo = max(0,h-(c[i2][j2]-50) );
	if(timetogo == 0)
		dfs(i2,j2);
	return;
}


void dij(int t , int x ,int y)
{
	qu.pop();
	if(mark[x][y])
		return ;
	mark[x][y]=true;
	d[x][y]=t;
	if(x < n-1)
		add(t,x,y,x+1,y);
	if(y < m-1)
		add(t,x,y,x,y+1);
	if(x >0)
		add(t,x,y,x-1,y);
	if(y >0)
		add(t,x,y,x,y-1);
}

void dfs(int x ,int y)
{
	mark2[x][y]=true;
	qu.push(pip(0,pii(x,y)));
	if(x < n-1)
		add2(x,y,x+1,y);
	if(y < m-1)
		add2(x,y,x,y+1);
	if(x >0)
		add2(x,y,x-1,y);
	if(y >0)
		add2(x,y,x,y-1);
}

int main()
{
	ios::sync_with_stdio(false);
	cin >> t;
	cout << fixed << setprecision(1);
	For(it,0,t)
	{
		cin >> h >> n >> m;
		For(i,0,n)
			For(j,0,m)
				cin >> c[i][j];
		For(i,0,n)
			For(j,0,m)
				cin >> f[i][j];
		d[n-1][m-1]=inf;
		For(i,0,n)
			For(j,0,m)
				mark[i][j] =mark2[i][j]= false;
		dfs(0,0);
		while(!qu.empty())
			dij(qu.top().fr,qu.top().sc.fr,qu.top().sc.sc);
		cout << "Case #" << it+1 << ": " << d[n-1][m-1]/10.0 << endl ;

	}
	return 0;
}
