#include <iostream>
#include <fstream>
using namespace std;

void updateMove( int &OnePosition, int &OneLastMove, int TwoPosition, int TwoLastMove, int button )
{
	int distance = abs(button - OnePosition);
	int nextTime = max( TwoLastMove + 1, OneLastMove + distance + 1);
	OneLastMove = nextTime;
	OnePosition = button;
}

int main()
{
	ifstream input( "c:\\inputs\\A-large.in" );
	ofstream output( "c:\\inputs\\A.out" );
	int T = 0;
	input >> T;
	int N;
	char color;
	int button;

	for ( int i = 0; i < T; ++i )
	{
		int OPosition = 1;
		int OLastMove = 0;
		int RPosition = 1;
		int RLastMove = 0;

		input >> N;
		for (  int j = 0; j < N; ++j )
		{
			input >> color;
			input >> button;
			if ( 'O' == color )
			{
				updateMove( OPosition, OLastMove, RPosition, RLastMove, button );
			}
			else
			{
				updateMove( RPosition, RLastMove, OPosition, OLastMove, button );
			}
		}
		output << "Case #" << i+1<< ": " << max( OLastMove, RLastMove ) << endl;

	}
	return 0;
}