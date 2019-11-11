#include <iostream>
#include <fstream>
using namespace std;

#define IN_PATH ("C:\\codejam\\1c\\A-large.in")
#define OUT_PATH ("C:\\codejam\\1c\\A-large.out")

#define MAX_N (1024)

bool intersect( int x1, int y1, int x2, int y2 )
{
	if ( x1 < x2 && y1 > y2 )
		return true;
	if ( x1 > x2 && y1 < y2 ) 
		return true;
	return false;
}

int main()
{
	ifstream infile ( IN_PATH );
	
	long long T;
	int A[MAX_N];
	int B[MAX_N];
	int currentB[ MAX_N];
	infile >> T;

	ofstream outfile ( OUT_PATH);

	for ( long caseNum = 1; caseNum <= T; ++caseNum )
	{
		int N;
		infile >> N;
		for ( int i = 0; i < N; ++i )
		{
			infile >> A[i] >> B[i];
		}

		int counter = 0;
		for ( int i = 0; i < N; ++i )
		{
			for (int j = i+1; j < N; ++j )
			{
				if ( i == j )
					continue; 

				if ( intersect( A[i], B[i], A[j], B[j] ) )
				{
					counter++;
				}
			}
		}

		outfile << "Case #" << caseNum << ": " << counter << endl;

	}
	outfile.close();
	return 0;
}