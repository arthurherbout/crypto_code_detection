#include <iostream>
#include <fstream>
using namespace std;

#define IN_PATH ("C:\\codejam\\C-small-attempt0.in")
#define OUT_PATH ("C:\\codejam\\C-small-attempt0.out")

#define MAX_N (505)
#define MODULO_VAL (100003)

long chooseTable[ MAX_N+1][MAX_N+1];
long table[MAX_N+1][MAX_N+1];

void fillChooseTable()
{
	// n =0
	chooseTable[0][0] = 1;
	for ( int i = 1; i < MAX_N; ++i )
	{
		chooseTable[0][i] = 0;
	}

	//n>=1
	for ( int n = 1; n < MAX_N; ++n )
	{
		for ( int k = 0; k <= n; ++k )
		{
			if ( k == 0 )
			{
				chooseTable[n][0] = 1;
			}
			else
			{
				chooseTable[n][k] = ( chooseTable[n-1][k] + chooseTable[n-1][k-1] ) % MODULO_VAL;
			}
		}
	}
}

long choose( int n, int k )
{
	if ( k > n )
		return 0;
	if ( n <0 || k < 0)
		return 0;
	return chooseTable[n][k];
}

int main()
{
	ifstream infile ( IN_PATH );

	fillChooseTable();

	int T;

	infile >> T;
	ofstream outfile ( OUT_PATH);

	for ( int caseNum = 1; caseNum <= T; ++caseNum )
	{
		int N;
		infile >> N;

		// init table
		for ( int n = 1; n <= N; ++n )
		{
			table[1][n] = 1;
		}

		//fill table
		for ( int s = 2; s <= N-1; ++s )
		{
			for ( int n = 0; n <= s; ++n )
			{
				table[s][n] = 0;
			}

			for ( int n = s+1; n <= N; ++n )
			{
				table[s][n] = 0;
				for ( int i = 1; i <= s-1; ++i )
				{
					table[s][n] = (table[s][n] + (table[i][s] * choose( n-s-1, s-i-1 ) ) % MODULO_VAL ) % MODULO_VAL;
				}
			}
		}

		long sum = 0;
		for ( int s = 1; s <= N-1; ++s )
		{
			sum = (sum + table[s][N]) % MODULO_VAL;
		}

		outfile << "Case #" << caseNum << ": " << sum << endl; 
	}
	outfile.close();
	return 0;
}