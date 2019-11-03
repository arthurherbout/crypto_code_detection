#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

#define T_SIZE	4		// table size is 4x4

// Read next test case from input file, and write solution for it into out file
void SolveTest(ifstream& inFile, ofstream& outFile, int testNum)
{
	int i,j;
	int r1;	// row 1
	int r2;	// row 2
	int m1[T_SIZE][T_SIZE];		// table 1
	int m2[T_SIZE][T_SIZE];		// table 2


	// ------------------------  Read Table 1
	inFile >> r1;
	if ((r1 > T_SIZE) || (r1 < 1))
	{
		outFile << "Bad Input r1 !" << r1 << "\n";
		return;
	}
	r1--;	// convert to 0-based idx

	for (i=0; i<T_SIZE; i++)
		for (j=0; j<T_SIZE; j++)
			inFile >> m1[i][j];
		
	// ------------------------  Read Table 2
	inFile >> r2;
	if ((r2 > T_SIZE) || (r2 < 1))
	{
		outFile << "Bad Input r2 !" << r2 << "\n";
		return;
	}
	r2--;	// convert to 0-based idx

	for (i=0; i<T_SIZE; i++)
		for (j=0; j<T_SIZE; j++)
			inFile >> m2[i][j];
		

	// ------------------------ Calc solution: -------------

	int card;
	
	card = 0;	// 0 means no matched card found
	for (i=0; i<T_SIZE; i++)
	{
		for (j=0; j<T_SIZE; j++)
		{
			// compare two rows, one from 1st table, 2nd from 2nd table
			if (m1[r1][i] == m2[r2][j])
			{
				if (0 == card)
					card = m1[r1][i];
				else 
					card = -1;	// Bad magician - more than 1 card applicable!
			}
		}
	}

	if (0 == card)
		outFile << "Case #" << testNum << ": Volunteer cheated!\n";
	else if (-1 == card)
		outFile << "Case #" << testNum << ": Bad magician!\n";
	else
		outFile << "Case #" << testNum << ": " << card << "\n";
}

int main()
{
	int T;	// number of Test cases
	int i;
	ifstream inFile("in.txt");
	ofstream outFile;
	outFile.open("out.txt"); 
	
	if (!inFile.is_open())
	{
		outFile << "Input file not found!\n";
		outFile.close();
		return 1;
	}

	inFile >> T;	// read number of tests
	for (i = 0; i < T; i++)
	{
		SolveTest(inFile, outFile, i+1);
	}
	outFile.close();

	return 0;
}

