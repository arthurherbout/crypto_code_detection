#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <iomanip>

using namespace std;

int main()
{
	// ifstream inf("in.txt");
	ifstream inf("C-small-attempt1.in");
	// ifstream inf("A-large.in");
	ofstream outf("out.txt");

	outf.setf(ios::fixed, ios::floatfield);
	outf.precision(7);
	
	int t;
	int n;
	int c;
	double e;

	inf >> t;

	for (int i = 0; i < t; i++)
	{
		inf >> n >> c;

		e = (double) (n + c) / (double) c;

		outf << "Case #" << (i + 1) << ": " << e << "\n";
	}
}