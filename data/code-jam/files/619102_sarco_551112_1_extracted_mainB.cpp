#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

#define IN_PATH ("C:\\codejam\\1c\\B-large.in")
#define OUT_PATH ("C:\\codejam\\1c\\B-large.out")

long long computeNumOfSearches( long long L, long long P, long long C )
{
	long double intervalSize = 0;
	long long temp = L;
	while ( temp < P )
	{
		intervalSize += 1;
		temp *= C;
	}
	long double two = 2;
	return ceil( log( intervalSize ) /  log(two) );
}

int main()
{
	ifstream infile ( IN_PATH );

	long long T;
	infile >> T;

	ofstream outfile ( OUT_PATH);

	for ( long caseNum = 1; caseNum <= T; ++caseNum )
	{
		long long L, P, C;
		infile >> L >> P >> C;

		long long numOfSearches = computeNumOfSearches( L, P, C );
		outfile << "Case #" << caseNum << ": " << numOfSearches << endl;

	}
	outfile.close();
	return 0;
}