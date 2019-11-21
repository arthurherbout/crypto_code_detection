#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main (void)
{
	ifstream infile("a.in");
	ofstream outfile("a2.out");

	int T;
	infile >> T;

	unsigned long vinePos[10005];
	unsigned long vineLen[10005];
	unsigned long dist[10005];

	for (int t = 1; t <= T; t++)
	{
		int numVines;
		infile >> numVines;

		for (int v = 0; v < numVines; v++)
		{
			infile >> vinePos[v];
			infile >> vineLen[v];
		}

		unsigned long D;
		infile >> D;

		bool canReach = false;

		dist[0] = vinePos[0] * 2;

		unsigned long maxDist = 0;
		unsigned long tempDist = 0;

		if (dist[0] >= D)
			canReach = true;

		for (int i = 1; i < numVines; i++){
			maxDist = 0;
			for (int j = 0; j < i; j++){
				if (dist[j] >= vinePos[i])
				{
					tempDist = vinePos[i] - vinePos[j];
					if (tempDist > vineLen[i]) tempDist = vineLen[i];
					tempDist += vinePos[i];

					if (tempDist > maxDist)
						maxDist = tempDist;
				}
			}
			if (maxDist >= D){
				canReach = true;
				break;
			}
			dist[i] = maxDist;
		}
		
		if (canReach)
			outfile << "Case #" << t << ": YES" << endl;
		else
			outfile << "Case #" << t << ": NO" << endl;
	}	

	return 0;
}