#include <iostream>
#include <fstream>
using namespace std;

#define IN_PATH ("C:\\codejam\\B-large.in")
#define OUT_PATH ("C:\\codejam\\B-large.out")

#define MAX_N (55)
#define MAX_B (1000000005)

void swap( long &x, long &y )
{
	long temp = x;
	x = y;
	y = temp;
}

void sortByX( long *X, long *V, long N )
{
	for ( long i= 0; i < N; ++i )
	{
		long minVal = X[i];
		long minLocation = i;
		for ( long j = 0; j < N; ++j )
		{
			if ( X[j] < minVal  )
			{
				minVal = X[j];
				minLocation = j;
			}			
		}
		if ( i != minLocation )
		{
			swap( X[i], X[minLocation]);
			swap(V[i], V[minLocation] );
		}
	}
}

long countSwapsForFirstKPassing( long * X, long * V, long K, long B, long T, long N )
{	
	int numOfCandidates = 0;
	bool foundEnoughtCandidates = false;
	int startLocation;
	for ( int i = N-1; i >= 0; --i )
	{
		if ( X[i] + V[i] * T >= B )
		{
			numOfCandidates ++;
		}
		if ( numOfCandidates >= K )
		{
			startLocation = i;
			foundEnoughtCandidates = true;
			break;
		}
	}

	if ( !foundEnoughtCandidates )
	{
		return -1;
	}

	int factor = 0;
	int counter = 0;
	for ( int i = startLocation; i < N; ++i )
	{
		if ( X[i] + V[i] * T >= B )
		{
			factor++;
		}
		else
		{
			counter += factor;
		}		
	}

	return counter;
}

int main()
{
	ifstream infile ( IN_PATH );
	
	long C;
	long X[MAX_N];
	long V[MAX_N];
	infile >> C;

	ofstream outfile ( OUT_PATH);

	for ( long caseNum = 1; caseNum <= C; ++caseNum )
	{
		long N, K, B, T;
		infile >> N >> K >> B >> T;
		for ( long i = 0; i < N; ++i )
		{
			infile >> X[i];
		}
		for ( long i = 0; i < N; ++i )
		{
			infile >> V[i];
		}

		//sortByX( (long *) X, (long *) V, N );
		long numOfSwaps = countSwapsForFirstKPassing( (long *) X, (long *) V, K, B, T, N );

		outfile << "Case #" << caseNum << ": "; 
		if ( numOfSwaps >= 0 )
		{
			outfile << numOfSwaps;
		}
		else
		{
			outfile << "IMPOSSIBLE";
		}
		outfile<<endl;
	}
	outfile.close();
	return 0;
}