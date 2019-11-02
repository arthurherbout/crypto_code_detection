#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <gmpxx.h>	/* GNU MP, http://gmplib.org/, link with -lgmp -lgmpxx */
#include <cstring>


using namespace std;


int main (int argc, char *argv[])
{
	int T, t;
	cin >> T;
	for (t = 1; t <= T; t++)
	{
		int R, C;
		cin >> R >> C;
		string line;
		char b[512][512];
		memset(b, 0, 512*512);
		for (int r = 0; r < R; r++)
		{
			string line;
			cin >> line;
			for (int c = 0; c < C; c+= 4)
			{
				int val = 0;
				int x = c/4;
				if (line[x] >= '0' && line[x] <= '9')
					val = line[x] - '0';
				else
					val = (line[x] - 'A') + 10;
				//cout << "line[x] is " << line[x] << " val is " << val << endl;
				if (val & 8) b[r][c] = 1;
				if (val & 4) b[r][c+1] = 1;
				if (val & 2) b[r][c+2] = 1;
				if (val & 1) b[r][c+3] = 1;
			}
		}
		
		/*for (int r = 0; r < R; r++)
		{
			for (int c = 0; c < C; c++)
				cout << (char)(b[r][c] + '0');
			cout << endl;
		}*/
		int sizes[513];
		for (int i = 0; i < 513; i++)
			sizes[i] = 0;
		int cutout = 0;
		int boardsize = R*C;
		while (cutout < boardsize)
		{
			int bestdim = 0, bestr = 0, bestc = 0;
			char test[512][512];
			memcpy(test, b, 512*512);
			for (int r = 0; r < R; r++)
			{
				for (int c = 0; c < C; c++)
				{
					if (test[r][c] > 1)
						continue;
					if (b[r][c] > 1)
						continue;
					int hfault = 0, vfault = 0, cursize = 1;
					char start = b[r][c];
					while (!hfault && !vfault)
					{
						for (int h = 0; h < cursize + 1; h++)
						{
							int expect = (int)start;
							for (int x = 0; x < cursize + h; x++)
							{
								if (expect) expect = 0;
								else expect = 1;
							}
							if (r+h >= R || c+cursize >= C || (int)(b[r+h][c+cursize]) != expect)
							{
								hfault = 1;
								for (int c2 = c+1; c2 < c+cursize && (c2) < C; c2++)
								{
									test[r][c2] = 2;
								}
								/*if (cursize > 3)
									cout << "Breaking horizontal at r " << r << " c " << c << " cursize " << cursize << " h " << h << endl;*/
								break;
							}
						}
						if (hfault) break;
						for (int v = 0; v < cursize; v++)
						{
							int expect = (int)start;
							for (int x = 0; x < cursize + v; x++)
							{
								if (expect) expect = 0;
								else expect = 1;
							}
							if (c+v >= C || r+cursize >= R || (int)(b[r+cursize][c+v]) != expect)
							{
								vfault = 1;
								for (int r2 = r+1; r2 < r+cursize && (r2) < R; r2++)
								{
									test[r2][c] = 2;
								}
								/*if (cursize > 3)
									cout << "Breaking vertical at r " << r << " c " << c << " cursize " << cursize << " v " << v << endl;*/
								break;
							}
						}
						if (vfault) break;
						cursize++;
					}
					if (cursize > bestdim)
					{
						bestdim = cursize;
						bestr = r;
						bestc = c;
					}
				}
			}
			
			/* Best is found now */
			//cout << "bestdim " << bestdim << " bestr " << bestr << " bestc " << bestc << endl;
			sizes[bestdim] += 1;
			for (int r = bestr; r < bestr+bestdim; r++)
				for (int c = bestc; c < bestc+bestdim; c++)
					b[r][c] = 2;
			cutout += bestdim*bestdim;
		}
		
		int nsizes = 0;
		for (int i = 1; i <= 512; i++)
			if (sizes[i]) nsizes++;
	
		
		cout << "Case #" << t << ": " << nsizes << endl;
		for (int i = 512; i > 0; i--)
		{
			if (sizes[i])
			{
				cout << i << " " << sizes[i] << endl;
			}
		}
	}
	
	return 0;
}


