#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	ifstream input( "c:\\inputs\\C-large.in" );
	ofstream output( "c:\\inputs\\C.out" );
	int T = 0;
	input >> T;
	int N;

	for ( int i = 0; i < T; ++i )
	{
		input >> N;
		long long min;
		input >> min;
		int temp;
		int XoredSum = min;
		long long sum = min;
		for (  int j = 1; j < N; ++j )
		{
			input >> temp;
			if ( temp<min)
			{
				min = temp;
			}
			XoredSum ^= temp;
			sum+= temp;
		}

		output << "Case #" << i+1<< ": ";
		if ( XoredSum != 0 )
		{
			output << "NO";
		}
		else
		{
			output << sum - min;
		}

		output << endl;
	}
	return 0;
}