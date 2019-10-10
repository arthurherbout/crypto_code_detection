
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <iterator>
#include <algorithm>
#include <queue>
#include <functional>
#include <sstream>
#include <complex>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <iomanip>
#using <mscorlib.dll>

using namespace std;

ifstream in("A-small-attempt2.in.txt");
ofstream out("A-small-attempt2.out.txt");
bool bound[300][300];
bool pocketp[300][300];
int miny[300];
int minx[300];
int maxy[300];
int maxx[300];
int offy[] = {-1, 0, 1, 0};
int offx[] = {0, 1, 0, -1};

int px[50000];
int py[50000];

bool is_inside(int vcount,int xthis,int ythis)
{
	int count=0;
	int i;
	/*
	说明限制：
	x[0]=x[vcount],y[0]=y[vcount]
	点在边上不算做在内部
	*/
	for (i=0;i<vcount;++i)
	{	
		if ((xthis-px[i])*(py[i+1]-py[i])==(ythis-py[i])*(px[i+1]-px[i]) && xthis<=max(px[i],px[i+1]) && ythis<=max(py[i],py[i+1]) && xthis>=min(px[i],px[i+1]) && ythis>=min(py[i],py[i+1]))
			return false;
		if (py[i]==py[i+1])
			continue;
		if ((ythis==py[i] && py[i]>py[i+1] && xthis>px[i]) || (ythis==py[i+1] && py[i]<py[i+1] && xthis>px[i+1]))
			++count;
		//下面的else需保证边的端点不在L(左射线)上
		else if (ythis>min(py[i],py[i+1]) && ythis<max(py[i],py[i+1]) && xthis>px[i]+((double)ythis-py[i])/(py[i+1]-py[i])*(px[i+1]-px[i]))
			++count;
	}
	return (count%2==1);
}

int _tmain()
{
	int cases;
	in >> cases;
	for (int i = 1; i <= cases; ++i)
	{
		cout << i << endl;
		int l;
		in >> l;
		string s;
		int t;
		string com;
		for (int j = 0; j < l; ++j)
		{
			in >> s >> t;
			for (int k = 0; k < t; ++k)
			{
				com += s;
			}
		}
		memset(bound, false, sizeof(bound));
		memset(pocketp, false, sizeof(pocketp));
		int nowy = 150;
		int nowx = 150;
		int nowdir = 0;
		int indexp = 0;
		for (int j = 0; j < 300; ++j)
		{
			miny[j] = minx[j] = 500;
			maxy[j] = maxx[j] = -1;
		}
		bound[150][150] = true;
		minx[150] = miny[150] = maxx[150] = maxy[150] = 150;
		px[0] = py[0] = 150;
        for (int j = 0; j < com.size(); ++j)
		{
			if (com[j] == 'L')
			{
				nowdir = (nowdir + 3) % 4;
			}
			else if (com[j] == 'R')
			{
				nowdir = (nowdir + 1) % 4;
			}
			else if (com[j] == 'F')
			{
				int nexty = nowy + offy[nowdir];
				int nextx = nowx + offx[nowdir];
				bound[nexty][nextx] = true;
				minx[nexty] = min(minx[nexty], nextx);
				maxx[nexty] = max(maxx[nexty], nextx);
				miny[nextx] = min(miny[nextx], nexty);
				maxy[nextx] = max(maxy[nextx], nexty);
				nowy = nexty;
				nowx = nextx;
				indexp++;
				px[indexp] = nowx;
				py[indexp] = nowy;
			}
		}
		for (int y = 0; y < 300; ++y)
		{
			for (int x = 0; x < 300; ++x)
			{
				if (!bound[y][x])
				{
					bool north = false;
					for (int oy = y - 1; oy > 0; --oy)
					{
						if (bound[oy][x]) north = true;
					}
					bool south = false;
					for (int oy = y + 1; oy < 300; ++oy)
					{
						if (bound[oy][x]) south = true;
					}
					bool east = false;
					for (int ox = x + 1; ox < 300; ++ox)
					{
						if (bound[y][ox]) east = true;
					}
					bool west = false;
					for (int ox = x - 1; ox > 0; --ox)
					{
						if (bound[y][ox]) west = true;
					}
					if ((north && south) || (east && west))
					{
						if (is_inside(indexp,x, y))
						{
							//out << y << " " << x << endl;
							pocketp[y][x] = true;
						}
					}
				}
			}
		}
		int res = 0;
        for (int y = 0; y < 299; ++y)
		{
			for (int x = 0; x < 299; ++x)
			{
				bool b1 = bound[y][x] || (pocketp[y][x]);
				bool b2 = bound[y+1][x] || (pocketp[y+1][x]);
				bool b3 = bound[y][x+1] || (pocketp[y][x+1]);
				bool b4 = bound[y+1][x+1] || (pocketp[y+1][x+1]);
				bool b5 = pocketp[y][x] || pocketp[y+1][x] || pocketp[y][x+1] || pocketp[y+1][x+1];
				bool b6 = b1 && b2 && b3 && b4 && b5;
				if (b6)
				{
					++res;
				}
			}
		}
		out << "Case #" << i << ": " << res << endl;
	}
    return 0;
}