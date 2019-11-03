
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

ifstream in("C-small-attempt0.in.txt");
ofstream out("C-small-attempt0.out.txt");
int occ[100];
int best[15][1 << 10];
int bcount[1 << 10];

int height;
int width;

int _tmain()
{
	for (int i = 0; i < (1 << 10); ++i)
	{
		bcount[i] = 0;
		for (int j = 0; j < 20; ++j)
		{
			if ((i & (1 << j)) != 0)
			{
				bcount[i]++;
			}
		}
	}
	int cases;
	in >> cases;
	for (int i = 1; i <= cases; ++i)
	{
		in >> height >> width;
		memset(occ, false, sizeof(occ));
		char ch;
		for (int j = 1; j <= height; ++j)
		{
			for (int k = 0; k < width; ++k)
			{
				in >> ch;
				if (ch == 'x')
				{
					occ[j] |= (1 << k);
				}
			}
		}
		memset(best, 0, sizeof(best));
		for (int j = 1; j <= height; ++j)
		{
			for (int stu = 0; stu < (1 << width); ++stu)
			{
				if ((occ[j] & stu) != 0)
				{
					best[j][stu] = -99999;
					continue;
				}
				bool con = true;
				for (int k = 0; k + 1 < width; ++k)
				{
					if ((stu & (1 << k)) != 0 && (stu & (1 << (k + 1))) != 0)
					{
						best[j][stu] = -99999;
						con = false;
						break;
					}
				}
				if (!con)
				{
					continue;
				}
				int nothave = 0;
				for (int k = 0; k < width; ++k)
				{
					if ((stu & (1 << k)) != 0 && k != 0)
					{
						nothave |= (1 << (k - 1));
					}
					if ((stu & (1 << k)) != 0 && k + 1 < width)
					{
						nothave |= (1 << (k + 1));
					}
				}
				int canhave = (((1 << width) - 1) & (~nothave) & (~occ[j - 1]));
				//if (stu == 5) out << canhave << "!";
				for (int tr = canhave; true; tr = (tr - 1) & canhave)
				{
					best[j][stu] = max(best[j][stu], best[j - 1][tr] + bcount[stu]);
					if (tr == 0)
					{
						break;
					}
				}
				//out << j << " " << stu << " " << best[j][stu] << endl;
			}
		}
		int result = 0;
		for (int j = 0; j < (1 << width); ++j)
		{
			result = max(result, best[height][j]);
		}
        out << "Case #" << i << ": " << result << endl;
	}
    return 0;
}