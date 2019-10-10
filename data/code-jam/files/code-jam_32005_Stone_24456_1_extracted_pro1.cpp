//PRO1
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

using namespace std;

ifstream in("A-large.txt");
ofstream out("A-largeout.txt");

int _tmain()
{
	int cases;
	in >> cases;
	for (int i = 1; i <= cases; ++i)
	{
		out << "Case #" << i << ": " << endl;
		vector<int> noth;
		vector<int> notw;
		int n;
		in >> n;
		int minh = 9999999;
		int maxh = -1;
		int minw = 9999999;
		int maxw = -1;
		for (int j = 0; j < n; ++j)
		{
			int h, w;
			string x;
			in >> h >> w;
			getline(in, x);
			if (x.find("NOT") == string::npos)
			{
				minh = min(minh, h);
				maxh = max(maxh, h);
				minw = min(minw, w);
				maxw = max(maxw, w);
			}
			else
			{
				noth.push_back(h);
				notw.push_back(w);
			}
		}
		//out << minh << " " << maxh << " " << minw << " " << maxw << endl;
		int m;
		in >> m;
		for (int j = 0; j < m; ++j)
		{
			int h, w;
			in >> h >> w;
			if (minh == 9999999)
			{
				bool booo = false;
				for (int kk = 0; kk < noth.size(); ++kk)
				{
					if (h == noth[kk] && w == notw[kk])
					{
						out << "NOT BIRD" << endl;
						booo = true;
						break;
					}
				}
				if (!booo)
				{
					out << "UNKNOWN" << endl;
				}
				continue;
			}
			if (h >= minh && h <= maxh && w >= minw && w <= maxw)
			{
				out << "BIRD" << endl;
				continue;
			}
			bool nb = false;
			for (int k = 0; k < noth.size(); ++k)
			{
				int th = noth[k];
				int tw = notw[k];
				if (th >= minh && th <= maxh && tw > maxw && w >= tw)
				{
					nb = true;
				}
				if (th >= minh && th <= maxh && tw < minw && w <= tw)
				{
					nb = true;
				}
				if (tw >= minw && tw <= maxw && th > maxh && h >= th)
				{
					nb = true;
				}
				if (tw >= minw && tw <= maxw && th < minh && h <= th)
				{
					nb = true;
				}
				if (th > maxh && tw > maxw && h >= th && w >= tw)
				{
					nb = true;
				}
				if (th > maxh && tw < minw && h >= th && w <= tw)
				{
					nb = true;
				}
				if (tw > maxw && th < minh && w >= tw && h <= th)
				{
					nb = true;
				}
				if (th < minh && tw < minw && h <= th && w <= tw)
				{
					nb = true;
				}
			}
			if (nb)
			{
				out << "NOT BIRD" << endl;
			}
			else
			{
				out << "UNKNOWN" << endl;
			}
		}
	}
	return 0;
}