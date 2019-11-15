//#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>
#include <math.h>


using namespace std;
using std::string;


ifstream fin("input.txt");
ofstream fout("output.txt");


void solve(int num)
{
	string s;
	
	int i;
	int p, k, l;
	int freq[1000];
	
	int cnt = 0;
	
	fin >> p >> k >> l;
	
	for (i = 0; i < l; i++) {
		fin >> freq[i];
	}
	
	cout << 0%k << endl;
	sort(freq, freq+l);
	
	int round = 0;
	for (i = 0; i < l; i++) {
		if ((i) % k == 0)
			round ++;
		cnt += freq[l-i-1] * round; 
	}
	

	fout << "Case #" << num << ": " << cnt << endl;
}

int main()
{
	int N;
	
	fin >> N;
	
	for(int i = 0; i < N; i++) {
		solve(i+1);
	}
	
	return 0;
}
