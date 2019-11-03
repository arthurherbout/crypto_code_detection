#include <iostream>
#include <fstream>
#include <string>

using namespace std;

ifstream in("C-small-attempt2.in");
ofstream out("out.txt");
int CASES = 1; 
int N, M, K;
int TOTENC;
int STONES = 0;

void Debug()
{
	cout << N << " " << M << " " << K << endl;
}

int MIN()
{
	if (N < M)
		return N;
	else
		return M;
}


int main()
{
	in >> CASES;
	int MaxW, MaxL;
	for (int i = 1; i <= CASES; i++)
	{
		
		out << "Case #" << i << ": ";
		in >> N;
		in >> M;
		in >> K;
		if (MIN() < 3 || K < 5)
		{
			out << K << endl;
		}
		else
		{
			MaxL = N - 2;
			MaxW = M - 2;
			int MINSTONES = 999;
			for (int j = 1; j <= MaxL; j++)
			{
				for (int k = 1; k <= MaxW; k++)
				{
					TOTENC = (j * k) + 2 * (j + k);
					STONES = 2 * (j + k);
					if (TOTENC < K)
						STONES += (K - TOTENC);

					if (STONES < MINSTONES)
						MINSTONES = STONES;						
				}
			}
			out << MINSTONES << endl;
		}

	}
	out.close();
	
}