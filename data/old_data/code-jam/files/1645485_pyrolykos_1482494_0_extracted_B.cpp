#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "fstream"
#include "string.h"

using namespace std;

int main (void)
{
	ifstream infile("B-small-attempt2.in");
	ofstream outfile("B.out");

	int T;
	infile >> T;

	for (int i = 0; i < T; i++)
	{
		int N;
		infile >> N;
		
		int oneStar[N];
		int twoStar[N];
	
		int starsGotten[N];
		int numStars = 0;

		for (int j = 0; j < N; j++)
		{
			infile >> oneStar[j];
			infile >> twoStar[j];
			
			starsGotten[j] = 0;
		}

		long long numLevelPlays = 0;

		bool stepForward = true;
		while (stepForward)
		{
			stepForward = false;
			for (int k = 0; k < N; k++)
			{
				if (starsGotten[k] == 0 && numStars >= twoStar[k])
				{
					stepForward = true;
					numStars += 2;
					starsGotten[k] = 2;
					numLevelPlays++;
				}
			}
			if (!stepForward)
			{
				for (int k = 0; k < N; k++)
				{
					if (starsGotten[k] == 1 && numStars >= twoStar[k])
					{
						stepForward = true;
						numStars++;
						starsGotten[k]++;
						numLevelPlays++;
						k = N+1;
						break;
					}
				}
			}
			if (!stepForward)
			{
				int bestOneToGet = -1;
				int highestTwoStarNum = -1;
				for (int k = 0; k < N; k++)
				{
					if (starsGotten[k] == 0 && numStars >= oneStar[k])
					{
						if (twoStar[k] > highestTwoStarNum)
						{
							highestTwoStarNum = twoStar[k];
							bestOneToGet = k;
						}
					}
				}
				if (bestOneToGet >= 0)
				{
					stepForward = true;
					numStars++;
					starsGotten[bestOneToGet]++;
					numLevelPlays++;
				}
			}
		}

		if (numStars < 2*N) 
			outfile << "Case #" << (i+1) << ": " << "Too Bad" << endl;
		else
			outfile << "Case #" << (i+1) << ": " << numLevelPlays << endl;

	}

	outfile.close();

	return 0;
}
