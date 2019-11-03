#include <iostream>
#include <fstream>
using namespace std;

#define IN_PATH ("C:\\codejam\\B-small-attempt1.in")
#define OUT_PATH ("C:\\codejam\\B-small-attempt1.out")

#define MAX_N (5000)
#define MAX_P (15)

int participate[MAX_P][MAX_N];
int costs[MAX_P][MAX_N];
void markPath( int x, int c, int P )
{
	int factor = 1 << P;
	int p = P-1;
	while( c > 0 )
	{
		int game = x / factor;
		participate[p][game] = 1;
		c--;
		p--;
		factor = factor >> 1;
	}
}

int main()
{
	ifstream infile ( IN_PATH );
	
	int T;
	int M[MAX_N];

	infile >> T;

	ofstream outfile ( OUT_PATH);

	for ( long caseNum = 1; caseNum <= T; ++caseNum )
	{
		int P, N;
		infile >> P;
		N = 1 << P;
		for ( long i = 0; i < N; ++i )
		{
			infile >> M[i];
			M[i] = P - M[i];
		}

		for ( int p = 0; p < P; ++p )
		{
			int n = 1 << (P-1 - p);
			for ( int i = 0; i < n; ++i )
			{
				infile >> costs[p][i];
				participate[p][i] = 0;
			}
		}

		for ( int i = 0; i < N; ++i )
		{
			markPath( i, M[i], P);
		}

		long gamesParticipated = 0;
		for ( int p = 0; p < P; ++p )
		{
			int n = 1 << (P-1 - p);
			for ( int i = 0; i < n; ++i )
			{
				gamesParticipated += participate[p][i];
			}
		}

		outfile << "Case #" << caseNum << ": " << gamesParticipated << endl; 
	}
	outfile.close();
	return 0;
}