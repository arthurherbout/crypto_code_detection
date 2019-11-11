//PRO4
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

ifstream in("dsmall.txt");
ofstream out("dsmallout.txt");
bool con1[110][110];
bool con2[110][110];

int _tmain()
{
	int cases;
	in >> cases;
	for (int i = 1; i <= cases; ++i)
	{
		memset(con1, false, sizeof(con1));
		memset(con2, false, sizeof(con2));
		int n;
		in >> n;
		for (int j = 0; j < n - 1; ++j)
		{
			int a, b;
			in >> a >> b;
			//out << a << b << endl;
			con1[a][b] = con1[b][a] = true;
		}
		int m;
		in >> m;
		for (int j = 0; j < m - 1; ++j)
		{
			int a, b;
			in >> a >> b;
			con2[a][b] = con2[b][a] = true;
			//out << a << b << endl;
		}
		vector<int> vc;
		for (int j = 1; j <= n; ++j)
		{
			vc.push_back(j);
		}
		bool ok = false;
		do
		{
			//out << "!";
			bool suc = true;
			for (int j = 0; j < m; ++j)
			{
				for (int k = 1; k <= m; ++k)
				{
					if (con2[j + 1][k] && (!con1[vc[j]][vc[k - 1]]))
					{
						suc = false;
						break;
					}
				}
				if (!suc) break;
			}
			if (suc)
			{
				ok = true;
				/*
				for (int j = 0; j < m; ++j)
				{
					out << vc[j] << endl;
				}
				*/
				break;
			}
		}while(next_permutation(vc.begin(), vc.end()));
		if (ok)
		{
			out << "Case #" << i << ": YES" << endl;
		}
		else
		{
			out << "Case #" << i << ": NO" << endl;
		}
	}
    return 0;
}