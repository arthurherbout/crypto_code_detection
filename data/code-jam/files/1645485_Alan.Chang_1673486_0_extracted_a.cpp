#include <iostream>
#include <fstream>
#include <algorithm>
//#include <map>
//#include <string>
//#include <cmath>
#include <vector>

using namespace std;

int num_trials;

int current_length, total_length; // A and B
double p_right;
double p_right_product[100001]; // n-th index = product of p_right[i] index 1 to n

int main(int argc, const char* argv[])  {
    ofstream fout ("a.out");
    ifstream fin ("a.in");

	p_right_product[0] = 1;
	fin >> num_trials;
	
	for (int trial = 1; trial <= num_trials; trial++) {
		fin >> current_length >> total_length;
		for (int i = 1; i <= current_length; i++) {
			fin >> p_right;
			p_right_product[i] = p_right_product[i-1] * p_right;
			// not good on memory, but oh well...
		}
		
		double min_expected_key_strokes = total_length + 2; // this is if you give up and start over;
		
		for (int n = 0; n <= current_length; n++) {
			// this calculates expected key strokes if you delete so that you have n letters left, and then continue typing
			double expected_key_strokes = (current_length - n) + (total_length - n + 1) + (1 - p_right_product[n]) * (total_length + 1); 
			// current_length - n : backspaces
			// total_length - n + 1 : type the rest + enter
			// (1 - p_right_product[i]) * (total_length + 1) : if there was a mistake
			
			if (min_expected_key_strokes > expected_key_strokes) {
				min_expected_key_strokes = expected_key_strokes; //update!
			}
		}
		fout << "Case #" << trial << ": " << min_expected_key_strokes  << endl;
	}
	
}
