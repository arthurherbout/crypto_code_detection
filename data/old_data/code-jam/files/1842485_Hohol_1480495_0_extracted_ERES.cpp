#pragma comment(linker,"/STACK:300000000")
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4800)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <iomanip>
#include <memory.h>
#include <cstdio>
#include <sstream>
#include <deque>
#include <bitset>
#include <numeric>
#include <ctime>
#include <queue>

using namespace std;

#define show(x) cout << #x << " = " << (x) << endl;
#define fori(i,n) for(int i = 0; i < (n); i++)
#define forab(i,a,b) for(int i = (a); i <= (b); i++)
#define sz(v) int((v).size())
#define all(v) (v).begin(),(v).end()
const double pi = 3.1415926535897932384626433832795;
template<class T> T abs(const T &a) { return a >= 0 ? a : -a; };
template<class T> T sqr(const T &x) { return x * x; }
typedef pair<int,int> ii;
typedef long long ll;
///////////////////////////////////////

int n, m;
char a[10][10];
ii wh[10];
int caveCnt;
bool visited[10][10];

void dfs(int x, int y)
{
	if(visited[x][y])
		return;
	visited[x][y] = true;
	for(int i = -1; i <= 0; i++)
		for(int j = -1; j <= 1; j++)
			if(abs(i)+abs(j) == 1)
			{
				int tox = x+i;
				int toy = y+j;
				if(a[tox][toy] != '#')
					dfs(tox,toy);
			}	
}

int pow3(int n)
{
	int r = 1;
	fori(i,n)
		r *= 3;
	return r;
}

void move(int &x, int &y, int dx, int dy)
{
	if(a[x+dx][y+dy] != '#')
	{
		x += dx;
		y += dy;
	}
}

bool leadToCave(int x, int y, int mask, int cave, int len)
{
	fori(i,len)
	{
		int t = mask%3;
		mask /= 3;
		if(t == 0)
			move(x,y,1,0);
		else if(t == 1)
			move(x,y,0,-1);
		else if(t == 2)
			move(x,y,0,1);
	}
	return a[x][y]-'0' == cave;
}

bool good(int mask, int cave, int len)
{
	fori(i,n)         ///////////
		fori(j,m)
			if(visited[i][j])
			{
				if(!leadToCave(i,j,mask,cave,len))
					return false;
			}
	return true;
}

bool exists(int len, int cave)
{
	int t = pow3(len);
	fori(mask,t)
	{
		if(good(mask,cave,len))
			return true;
	}
	return false;
}

void work(int cave)
{
	memset(visited,0,sizeof(visited));
	dfs(wh[cave].first,wh[cave].second);
	int cnt = 0;
	fori(i,n)
		fori(j,m)
			if(visited[i][j])
				cnt++;
	cout << endl;
	cout << cave << ": " << cnt;
	bool ok = false;
	for(int len = 0; len <= 14; len++)
		if(exists(len,cave))
		{
			ok = true;
			break;
		}
	if(ok)
		cout << " Lucky";
	else
		cout << " Unlucky";
}

void work()
{
	cin >> n >> m;		
	caveCnt = 0;
	fori(i,n)
		fori(j,m)
		{	
			cin >> a[i][j];
			if(isdigit(a[i][j]))
			{
				wh[a[i][j]-'0'] = ii(i,j);
				caveCnt++;
			}
		}
	fori(i,caveCnt)
		work(i);
}

int main() 
{ 
#ifndef ONLINE_JUDGE
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
#endif
	int testCnt;
	cin >> testCnt;
	for(int testNo = 1; testNo <= testCnt; testNo++)
	{

		cout << "Case #" << testNo << ": ";		
		work();
		cout << endl;
	}
}