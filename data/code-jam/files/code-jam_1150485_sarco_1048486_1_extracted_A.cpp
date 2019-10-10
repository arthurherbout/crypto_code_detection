#include <iostream>
#include <fstream>
using namespace std;

#define IN_PATH ("C:\\Users\\User\\Documents\\Visual Studio 2008\\Projects\\CodeJam_Round1B_2011\\A-large.in")
#define OUT_PATH ("C:\\Users\\User\\Documents\\Visual Studio 2008\\Projects\\CodeJam_Round1B_2011\\A-large.out")

#define MAX_N (200)

int Games[MAX_N][MAX_N];
int winsArr[MAX_N];
int gamesArr[MAX_N];
double WP[MAX_N];
double OWP[MAX_N];
double OOWP[MAX_N];
double RPI[MAX_N];

void computeRPI()
{
}

int main()
{
	ifstream infile ( IN_PATH );

	int T;

	infile >> T;
	ofstream outfile ( OUT_PATH);
	outfile.precision(10);
	outfile.setf(ios::fixed,ios::floatfield);   // floatfield set to fixed


	for ( long long caseNum = 1; caseNum <= T; ++caseNum )
	{
		int N;
		infile >> N;

		char c;
		for ( int i = 0; i < N; ++i )
		{
			int wins = 0;
			int plays = 0;
			for ( int j = 0; j < N; ++j )
			{
				infile >> c;
				if ( '1' == c )
				{
					Games[i][j] = 1;
					wins++;
					plays++;
				}
				else if ( '0' == c )
				{
					Games[i][j] = 0;
					plays++;
				}
				else if ( '.' == c )
				{
					Games[i][j] = -1;
				}
			}
			winsArr[i] = wins;
			gamesArr[i] = plays;
			WP[i] = (double) winsArr[i] / (double) gamesArr[i];
		}

		for ( int i = 0; i < N; ++i )
		{
			OWP[i] = 0;
			for ( int j = 0; j < N; ++j )
			{							
				if ( Games[i][j] == 1 )
				{
					OWP[i] += ((double) winsArr[j]) / ((double) gamesArr[j] -1 );
				}
				else if ( Games[i][j] == 0 )
				{
					OWP[i] += ((double) winsArr[j] - 1) / ((double) gamesArr[j] -1) ;
				}
			}
			OWP[i] /= (double) gamesArr[i];
		}

		for ( int i = 0; i < N; ++i )
		{
			OOWP[i] = 0;
			for ( int j = 0; j < N; ++j )
			{							
				if ( Games[i][j] == 1 || Games[i][j] == 0)
				{
					OOWP[i] += OWP[j];
				}
			}
			OOWP[i] /= (double) gamesArr[i];
		}

		outfile << "Case #" << caseNum << ":" << endl; 	
		for ( int i = 0; i < N; ++i )
		{
			double RPI = 0.25 * WP[i] + 0.50 * OWP[i] + 0.25 * OOWP[i];
			outfile << RPI << endl; 	
		}

	}
	outfile.close();
	return 0;
}