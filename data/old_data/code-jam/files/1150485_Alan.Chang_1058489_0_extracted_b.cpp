#include <iostream>
#include <fstream>
#include <algorithm>
//#include <map>
//#include <string>
//#include <cmath>
#include <vector>

using namespace std;

int num_trials;
int num_pts_with_vendors;
int min_distance;
int num_vendors;
int num_vendors_left;

double time_so_far; 

int start_pt[200];
int start_pt_num_vendors[200];

int main(int argc, const char* argv[])  {
    ofstream fout ("b.out");
    ifstream fin ("b.in");

	fin >> num_trials;
	for (int trial = 1; trial <= num_trials; trial++) {
		fin >> num_pts_with_vendors;
		fin >> min_distance;
		num_vendors = 0;
		time_so_far = 0;
		 
		for (int i = 0; i < num_pts_with_vendors; i++) {
			fin >> start_pt[i] >> start_pt_num_vendors[i];
			num_vendors +=  start_pt_num_vendors[i];
		}
		
		int shift = start_pt[0];
		for (int i = 0; i < num_pts_with_vendors; i++) {
			start_pt[i] = start_pt[i] - shift;
			//cout << start_pt[i] << endl;
		}
		
		num_vendors_left = num_vendors; // need to get all of them;
		int first_available_position = 0;
		for (int i = 0; i < num_pts_with_vendors; i++) {
			double time_needed_to_spread_out = (double) (start_pt_num_vendors[i] - 1) * min_distance / 2;
			double time_needed_to_move_together;
			if (start_pt[i] > first_available_position) {
				// just need to spread out
				time_needed_to_move_together = 0;
				first_available_position = start_pt[i] + start_pt_num_vendors[i] * min_distance;
			} else {
				// need
				time_needed_to_move_together = (double) (first_available_position - start_pt[i]) / 2;
				first_available_position += start_pt_num_vendors[i] * min_distance;
			}
			
			double time_needed = time_needed_to_spread_out + time_needed_to_move_together;
			
			if (time_needed > time_so_far) {
				time_so_far = time_needed;
			}
			
			
		}
		
		cout << "Case #" << trial << ": " << time_so_far << endl;
		fout << "Case #" << trial << ": " << time_so_far << endl;
	}

}
