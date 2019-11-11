#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;
int main()
{
	ifstream fin("input.txt");
	ofstream fout("output.txt");
	int t;
	fin >> t;
	int r, n, m , k;
	fout << "Case #1: " << endl;
	for (int i=0; i<t; i++)
	{
		int temp;
		fin >> r >> n >> m >> k;
		for (int l=0; l<r;l++)
		{
			int max = 1;
			for (int j=0; j<k; j++)
			{
				fin >> temp;
				if (temp > max) max = temp;
 			}
			temp = m;
			while((max % temp !=0)||(max/temp<4))
			{
				if (temp ==2) break;
				temp --;
			}
			fout << temp;
			max = max / temp;
			while((max % temp !=0)||(max/temp<2))
			{
				if (temp ==2) break;
				temp --;
			}
			fout << temp;
			if (temp == 2) fout << 2 << endl;
			else
			fout << max / temp << endl;
		}
	}
	fin.close();
	fout.close();
	return 0;
}


				
					
