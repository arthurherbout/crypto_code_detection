#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int T;
int A,B,C,minPCandy = INT_MAX,Sum = 0;
int N;

int main(){
	ofstream outputFile;
	outputFile.open ("outputFile.txt", ofstream::out | ofstream::trunc);
	ifstream input;
	input.open("input.txt",ifstream::in);
	input >> T;
	for(int t=0;t< T;t++)
	{
		input >> N;

		Sum = 0;
		minPCandy = INT_MAX;

		input>>B;
		Sum += B;
		if (B<minPCandy)
			minPCandy = B;

		for(int i=0;i<N-1;i++)
		{
			input>>A;
			if (A<minPCandy)
			{
				minPCandy = A;
			}
			Sum += A;
			B ^= A;
		}
		if (B!=0)
			outputFile << "Case #"<<(t+1)<<": " << "NO" << endl;
		else 
			outputFile << "Case #"<<(t+1)<<": " << (Sum - minPCandy) << endl;
		
	}
	input.close();
	outputFile.close();
	return 0;
}