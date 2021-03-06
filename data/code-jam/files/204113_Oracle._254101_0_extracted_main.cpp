#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
using namespace std;

#pragma comment(linker,"/STACK:16777216")

const double eps = 1e-7;

char a[100][100];

set<vector<pair<int,int> > > res;

struct Triple
{
	int x,y;
	int dig;

	Triple(int X=0,int Y=0,int Dig=0)
	{
		x=X;y=Y;dig=Dig;
	}
};

int n,m,F;

inline bool valid(int x,int y)
{
	return (x>=0&&x<n&&y>=0&&y<m);
}

bool candigL(int x,int y)
{
	if (valid(x+1,y-1)&&valid(x,y-1))
	{
		if (a[x+1][y-1]=='#'&&a[x][y-1]=='.')
			return true;
	}
	return false;
}

bool candigR(int x,int y)
{
	if (valid(x+1,y+1)&&valid(x,y+1))
	{
		if (a[x+1][y+1]=='#'&&a[x][y+1]=='.')
			return true;
	}
	return false;
}

pair<int,int> moveL(int x,int y)
{
	if (!valid(x,y-1))
		return make_pair(-1,-1);

	if (a[x][y-1]=='#')
		return make_pair(-1,-1);

	int deep=0;
	++x;

	while (valid(x,y-1)&&a[x][y-1]=='.') { ++deep; ++x; }

	if (deep>F)
		return make_pair(-1,-1);

	return make_pair(x-1,y-1);
}

pair<int,int> moveR(int x,int y)
{
	if (!valid(x,y+1))
		return make_pair(-1,-1);

	if (a[x][y+1]=='#')
		return make_pair(-1,-1);

	int deep=0;
	++x;

	while (valid(x,y+1)&&a[x][y+1]=='.') { ++deep; ++x; }

	if (deep>F)
		return make_pair(-1,-1);

	return make_pair(x-1,y+1);
}

int R;

void solve(vector<pair<int,int> > v)
{
	res.insert(v);
	int x=v[0].first;
	int y=v[0].second;
	int dig=v.size()-1;

	if (x==n-1)
	{
		R=min(R,dig);
		return;
	}
	if (R<=dig)
		return;

	pair<int,int> next=moveL(x,y);
	int nx=next.first;
	int ny=next.second;
	int ndig=dig;

	if (nx!=-1)
	{
		vector<pair<int,int> > nv=v;
		nv[0].first=nx;
		nv[0].second=ny;

		if (!res.count(nv))
			solve(nv);
	}

	next=moveR(x,y);
	nx=next.first;
	ny=next.second;
	ndig=dig;

	if (nx!=-1)
	{
		vector<pair<int,int> > nv=v;
		nv[0].first=nx;
		nv[0].second=ny;

		if (!res.count(nv))
			solve(nv);
	}
	
	nx=-1;ny=-1;ndig=-1;
	if (candigL(x,y))
	{
		vector<pair<int,int> > nv=v;
		nv.push_back(make_pair(x+1,y-1));
		sort(nv.begin()+1,nv.end());

		if (!res.count(nv))
		{
			a[x+1][y-1]='.';
			solve(nv);
			a[x+1][y-1]='#';
		}
	}

	nx=-1;ny=-1;ndig=-1;
	if (candigR(x,y))
	{
		vector<pair<int,int> > nv=v;
		nv.push_back(make_pair(x+1,y+1));
		sort(nv.begin()+1,nv.end());

		if (!res.count(nv))
		{
			a[x+1][y+1]='.';
			solve(nv);
			a[x+1][y+1]='#';
		}
	}
}

int main()
{
	freopen("in.txt","r",stdin);
	freopen("out.txt","w",stdout);

	int T;
	scanf("%d",&T);
	for (int test=0;test<T;test++)
	{
		res.clear();
		scanf("%d%d%d\n",&n,&m,&F);
		for (int i=0;i<n;i++)
			gets(a[i]);
		
		R=10000;
		vector<pair<int,int> > v;
		v.push_back(make_pair(0,0));
		solve(v);

		if (R!=10000)
			cout<<"Case #"<<test+1<<": Yes "<<R<<endl; else
			cout<<"Case #"<<test+1<<": No"<<endl;
	}

	return 0;
}