#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

const int MN = 22;


ifstream inf("A-small-attempt0.in");
ofstream outf("A-small-attempt0.txt");


int main()
{

	int i, j, s;
	int N;
	int al[102];
	inf>>N;
	int re = 0;
	for ( s = 0; s < N; s++)
	{
		int P = 0, K = 0, L = 0;
		inf>>P;
		inf>>K;
		inf>>L;
		int tem = 0;
		for ( i = 0; i < L; i++)
		{
		
			inf>> al[i];
		}//for
		
		if ( L > (P*K))
		{
			outf<<"Case #"<<s+1<<": "<<"Impossible\n";
			continue;
		}

		int length = L;
		sort(al,al+L);
		reverse(al,al+L);
	
		int weig = 1;
		int flag = 0;
		re = 0;
		for ( i = 0 ; i < L; i++ )
		{
			if ( flag == K)
			{
				flag = 0;
				weig++;
			}
			re += al[i] * weig;
			flag++;
		}//for
	
		outf<<"Case #"<<s+1<<": "<<re<<"\n";
	
	}
	return 0;
}