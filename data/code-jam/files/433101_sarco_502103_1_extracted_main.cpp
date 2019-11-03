#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

#define IN_PATH ("c:\\codejam\\Q1\\1.in" )
#define OUT_PATH ("c:\\codejam\\Q1\\1.out" )

long long T=0, N=0, K=0;

int main()
{
	cout << "hello world" << endl;

	ifstream fin( IN_PATH );
	ofstream fout( OUT_PATH );
	fin >> T;
	long power;
	bool isOn = false;
	for ( int i =0; i < T; ++i )
	{
		fin >> N >> K;
		power = (1 << N) - 1;
		isOn = ( (power & K) == power );
		fout << "Case #" << i+1 << ": ";
		if ( isOn )
		{
			fout << "ON";
		}
		else
		{
			fout << "OFF";
		}
		if ( i < T-1 )
		{
			fout << endl;
		}

	}

	fout.close();
	fin.close();
	return 0;
}