#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*
 * a => y
 * b => h
 * c => e
 * d => s
 * e => o
 * f => c
 * g => v
 * h => x
 * i => d
 * j => u
 * k => i
 * l => g
 * m => l
 * n => b
 * o => k
 * p => r
 * q => z
 * r => t
 * s => n
 * t => w
 * u => j
 * v => p
 * w => f
 * x => m
 * y => a
 * z => q 
 */

 char codebook[26] = {'y', 'h', 'e', 's', 'o', 'c', 'v', 'x', 'd', 'u', 'i', 'g', 'l', 'b', 'k', 'r', 'z', 't', 'n', 'w', 'j', 'p', 'f', 'm', 'a', 'q'};

 int main(int argc, char **argv) {
	 if (argc < 2) {
		 cout << "Usage: "<<argv[0] << " inputFile" << endl;
		 exit(0);
	 }
	 ifstream inputFile;
	 inputFile.open(argv[1]);
	 int T;
	 string G;
	 inputFile >> T;
	 getline(inputFile, G);
	 for (int i = 0; i < T; i++) {
		 getline(inputFile, G);
		 int strlen = G.length();
		 for (int j = 0; j < strlen; j++) {
			 if (G[j] >= 'a' && G[j] <= 'z') {
				 G[j] = codebook[G[j] - 'a'];
			 }
		 }
		 cout << "Case #" << i + 1 <<": " << G << endl;
	 }
 }
