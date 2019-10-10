#include <iostream>
#include <fstream>
#include <limits.h>
using namespace std;

#define IN_PATH ("C:\\Users\\User\\Documents\\Visual Studio 2008\\Projects\\CodeJam_Round1B_2011\\B-large.in")
#define OUT_PATH ("C:\\Users\\User\\Documents\\Visual Studio 2008\\Projects\\CodeJam_Round1B_2011\\B-large.out")

#define MAX_C (205)
#define MAX_T (55)

long long C;
long long D;
long long P[MAX_C];
long long V[MAX_C];
long long LocationFirstV[MAX_C];
long long LocationLastV[MAX_C];
long long sums[MAX_C];
long long Max[MAX_T];
long long Min[MAX_T];

void updateMinMax( long long diff, long long &max, long long &min )
{
	if (diff > max )
	{
		max = diff;
	}
	if ( diff < min )
	{
		min = diff;
	}
}

int main()
{
	ifstream infile ( IN_PATH );

	int T;

	///////////////// get min max	
	infile >> T;
	for ( long long caseNum = 1; caseNum <= T; ++caseNum )
	{
		infile >> C >> D;
		for ( int i = 0; i < C; ++i )
		{
			infile >> P[i] >> V[i];
		}
		Max[caseNum] = P[0];
		Min[caseNum] = P[C-1];
	}

	//////////////////////////////
	infile.seekg(0);
	infile.clear();
	infile >> T;
	ofstream outfile ( OUT_PATH);


	for ( long long caseNum = 1; caseNum <= T; ++caseNum )
	{
		infile >> C >> D;

		long long sumV = 0;
		long long diff1, diff2;
		long long min = 0;
		long long max = 0;		
		for ( int i = 0; i < C; ++i )
		{
			infile >> P[i] >> V[i];
			LocationFirstV[i] = P[0] + sumV * D;
			sumV += V[i]-1;
			LocationLastV[i] = P[0] + sumV * D;
			sumV++;

			sums[i] = sumV;

			diff1 = LocationFirstV[i] - P[i];
			diff2 = LocationLastV[i] - P[i];
			updateMinMax( diff1, max, min );
			updateMinMax( diff2, max, min );
		}

		long long expectedDist; 
		long long sumInInterval;
		long long maxDist = 0;
		long long dist = 0;
		for ( int i = 0; i < C; ++i )
		{
			for ( int j =i; j < C; ++j )	
			{
				if ( i != 0 )
				{
					sumInInterval = sums[j] - sums[i-1];	
				}
				else
				{
					sumInInterval = sums[j];	
				}

				expectedDist = (sumInInterval-1) * D;
				dist = expectedDist - (P[j] - P[i]);
				if ( dist > maxDist )
				{
					maxDist = dist;
				}
			}
		}

		long long half = maxDist/2;
		outfile << "Case #" << caseNum << ": " << half;
		if ( maxDist % 2 == 1 )
		{
			outfile << ".5";
		}
		outfile << endl;
	}

	outfile.close();
	return 0;
}