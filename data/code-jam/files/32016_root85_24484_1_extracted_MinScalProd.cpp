#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

void main()
{
	ifstream in("A-large.in");
	ofstream out("product.out");

	int tc;
	in >>tc;
	for (int cc = 0;cc < tc;cc++)
	{
		int n;
		in >>n;
		vector<long long> a(n), b(n);

		for (int i = 0;i < n;i++)
			in >>a[i];
		for (int i = 0;i < n;i++)
			in >>b[i];

		sort(a.begin(), a.end());
		sort(b.begin(), b.end());

		long long min = 0;

		for (int i = 0;i < n;i++)
			min += a[i] * b[n - i - 1];

		out <<"Case #"<<cc + 1<<": "<<min <<endl;
	}
}