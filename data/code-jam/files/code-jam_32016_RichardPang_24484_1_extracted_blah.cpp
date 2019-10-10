#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
	ifstream fin("A-large.in");
	ofstream fout("A-large.out");
	int n;
	fin >> n;
	for (int c = 0; c < n; c++) {
		int r;
		fin >> r;
		vector<long long> v1(r), v2(r);
		for (int i = 0; i < r; i++)
			fin >> v1[i];
		for (int i = 0; i < r; i++)
			fin >> v2[i];
		sort(v1.begin(), v1.end());
		sort(v2.begin(), v2.end());
		
		long long res = 0;
		for (int i = 0; i < r; i++) 
			res += v1[i] * v2[r - 1 - i];
		fout << "Case #" << c + 1 << ": " << res << endl;
	}
	return 0;
}