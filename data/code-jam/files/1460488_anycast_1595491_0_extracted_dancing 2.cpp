#include <iostream>
#include <fstream>
#include <string>

using namespace std;

 int main(int argc, char **argv) {
	 if (argc < 2) {
		 cout << "Usage: "<<argv[0] << " inputFile" << endl;
		 exit(0);
	 }
	 ifstream inputFile;
	 inputFile.open(argv[1]);
	 int T;
	 inputFile >> T;
	 for (int i = 0; i < T; i++) {
		 int N, S, p, t, count = 0;
		 inputFile >> N;
		 inputFile >> S;
		 inputFile >> p;
		 for (int j = 0; j < N; j++) {
			 inputFile >> t;
			 int lower = t / 3;
			 int upper = lower;
			 int r = t % 3;
			 if (r > 0)
				 upper++;

			 if (upper >= p) {
				 count++;
				 continue;
			 }

			 if (S <= 0) {
				 continue;
			 }

			 switch (r) {
			 case 0: if (lower + 1 >= p && lower != 0) {
						count++;
						S--;
					 }
					 break;
			 case 1: break;
			 case 2: if (upper + 1 >= p) {
						count++;
						S--;
					 }
					 break;
			 }
		 }
		 cout << "Case #" << i + 1 <<": " << count << endl;
	 }
 }
