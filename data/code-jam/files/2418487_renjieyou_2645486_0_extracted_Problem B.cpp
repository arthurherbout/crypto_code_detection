#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int FindNextBiggest(int i, vector<int> &v);

int main()
{
	ifstream infile("B-small-attempt0.in");
	//ifstream infile("BInput.txt");
	ofstream outfile("ProblemBOutput.txt");

	int T;
	infile >> T;

	int E,R,N;
	int next_biggest;
	int distance;
	for (int case_num = 1; case_num <= T; case_num++) {
		infile >> E >> R >> N;
		vector<int> v_vec(N);
		for (int i = 0; i < N; i++)
			infile >> v_vec[i];

		int gain = 0;
		int energy = E;
		for (int i = 0; i < N; i++) {
			next_biggest = FindNextBiggest(i, v_vec);
			if (next_biggest == -1) {
				gain += v_vec[i]*energy;
				energy = 0;
			}
			else {
				distance = next_biggest - i;
				if (distance*R + energy > E) {
					gain += min(distance*R + energy - E, energy)*v_vec[i];
					energy -= min(distance*R + energy - E, energy);
				}
			}
			energy = min(energy + R,E);
		}
		outfile << "Case #" << case_num << ": " << gain << endl;
	}




	return 0;
}

int FindNextBiggest(int i, vector<int> &v)
{
	int pos = i+1;
	int base = v[i];
	while (pos < (int) v.size()) {
		if (v[pos] > base)
			return pos;
		pos++;
	}
	return -1;
}