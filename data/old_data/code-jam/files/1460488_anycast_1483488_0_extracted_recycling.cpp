#include <iostream>
#include <fstream>
#include <set>

using namespace std;

int rotate(int num, int d, int k) {
	int p = 1;
	int kk = d - k;
	while(k > 0) {
		p *= 10;
		k--;
	}
	int right = num % p;
	if (right < p/10 || right == 0)
		return -1;

	int left = num / p;

	int pp = 1;
	while (kk > 0) {
		pp *= 10;
		kk--;
	}

	return (right * pp + left);
}

int digits(int num) {
	int d = 0;
	do {
		num /= 10;
		d++;
	}while(num > 0);
	return d;
}

int main(int argc, char **argv) {
	 if (argc < 2) {
		 cout << "Usage: "<<argv[0] << " inputFile" << endl;
		 exit(0);
	 }
	 ifstream inputFile;
	 inputFile.open(argv[1]);
	 int T, A, B;
	 inputFile >> T;
	 for (int i = 0; i < T; i++) {
		 inputFile >> A;
		 inputFile >> B;
		 int count = 0;
		 for (int n = A; n <= B; n++) {
			int d =	digits(n);
			set<int> recycles;
			for (int j = 1; j < d; j++) {
				int res = rotate(n,d,j);
				if (res >= A && res <= B && res > n) {
					recycles.insert(res);
				}
			}
			count += recycles.size();
		 }
		 cout << "Case #" << i + 1 <<": " << count << endl;
	 }
 }
