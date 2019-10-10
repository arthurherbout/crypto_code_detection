#include <iostream>
#include <fstream>
#include <cmath>
#include <queue>

using namespace std;

#define IN_PATH ("c:\\codejam\\Q3\\3.in" )
#define OUT_PATH ("c:\\codejam\\Q3\\3.out" )
#define MAX_N (1003)
long long T=0, R=0, k=0, N=0;
queue<long> line;

int main()
{
	cout << "hello world" << endl;

	ifstream fin( IN_PATH );
	ofstream fout( OUT_PATH );
	fin >> T;
	for ( int i =0; i < T; ++i )
	{
		long sum = 0;
		fin >> R >> k >> N;
		long temp;
		while ( !line.empty() )
		{
			line.pop();
		}
		for (long j=0; j < N; ++j )
		{
			fin >> temp;
			line.push( temp);
		}

		for ( long round = 0; round < R; ++round )
		{
			long numOfPeopleOnBoard = 0;
			long numOfGroupsBoarded = 0;
			bool allBoarded = false;
			while (numOfPeopleOnBoard < k && numOfGroupsBoarded < N && !allBoarded )
			{
				long firstGroupSize = line.front();
				if ( numOfPeopleOnBoard + firstGroupSize <= k )
				{
					numOfPeopleOnBoard += firstGroupSize;
					sum += firstGroupSize;
					numOfGroupsBoarded ++;
					line.pop();
					line.push( firstGroupSize);
				}
				else
				{
					allBoarded = true;
				}
			}
		}

		fout << "Case #" << i+1 << ": " << sum;
		if ( i < T-1 )
		{
			fout << endl;
		}

	}

	fout.close();
	fin.close();
	return 0;
}