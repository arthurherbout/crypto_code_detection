#include <iostream>
#include <fstream>
#include <algorithm>
//#include <map>
//#include <string>
//#include <cmath>
#include <vector>

using namespace std;

const int MAX_DIM = 500;
int grid[MAX_DIM][MAX_DIM];
int row;
int col;
int d; //useless i think?

int num_trials;

int hori_sum[MAX_DIM][MAX_DIM][MAX_DIM]; // y, x, length
int vert_sum[MAX_DIM][MAX_DIM][MAX_DIM];


int main(int argc, const char* argv[])  {
    ofstream fout ("b.out");
    ifstream fin ("b.in");

	fin >> num_trials;
	
	for (int trial = 1; trial <= num_trials; trial++) {
		fin >> row >> col >> d; 
		//cout << "row = " << row << " col = " << col << endl;
		for (int y = 0; y < row; y++) {
			string next_row;
			fin >> next_row;
			for (int x = 0; x < col; x++) {
				grid[y][x] = next_row[x] - 48;
			}
		}

		

		//hori sum
		for (int y = 0; y < row; y++) {
			for (int x = 0; x < col; x++) {
				hori_sum[y][x][1] = grid[y][x];
				for (int length = 2; x + length <= col; length++) {
					hori_sum[y][x][length] = hori_sum[y][x][length-1] + grid[y][x+length-1];
				}
			}
		}

		//vert sum
		for (int x = 0; x < col; x++) {
			for (int y = 0; y < row; y++) {
				vert_sum[y][x][1] = grid[y][x];
				for (int length = 2; y + length <= row; length++) {
					vert_sum[y][x][length] = vert_sum[y][x][length-1] + grid[y+length-1][x];
				}
			}
		}
		
		int start_check = (row > col) ? col : row; // min;
		
		//cout << "trial = " << trial << " start_check = " << start_check << endl;
		int best = 0;
		
		for (int k = start_check; k >= 3; k--) {
			// here x,y represent top corner
			
			for (int y = 0; y + k <= row; y++) {
				for (int x = 0; x + k <= col; x++) {
					bool does_this_work = false;
					
					//cout << "k = " << k << " y = " << y << " x = " << x << endl;
					
					if (k % 2 == 1) {
						int hori = 0;
						for (int j = 1; j < k-1; j++) { //annoying edges...
					//		cout << " j = " << j << " hori_sum = " << hori_sum[y+j][x][k] << endl;
							 hori += (j - k/2)* hori_sum[y+j][x][k]; // integer division!!
						}
						hori += (0 - k/2)* hori_sum[y+0][x+1][k-2];
						hori += (k-1 - k/2)* hori_sum[y+k-1][x+1][k-2];
						
						
						int vert = 0;
						for (int i = 1; i < k-1; i++) {
					//		cout << " i = " << i << " vert_sum = " << vert_sum[y][x+i][k] << endl;
							 vert += (i - k/2)* vert_sum[y][x+i][k]; // integer division!!
						}
						vert += (0 - k/2)* vert_sum[y+1][x+0][k-2];
						vert += (k-1 - k/2)* vert_sum[y+1][x+k-1][k-2];
						
					//	cout << " h = " << hori << " v = " << vert << endl;
						
						if (hori == 0 && vert == 0) {
							does_this_work = true;
						}
					} else { // k is even
						int hori = 0;
						for (int j = 1; j < k-1; j++) {
							 hori += (2*j - k+1)* hori_sum[y+j][x][k]; // integer division!!
						}
						hori += (2*0 - k+1)* hori_sum[y+0][x+1][k-2];
						hori += (2*(k-1) - k+1)* hori_sum[y+k-1][x+1][k-2];
						
						int vert = 0;
						for (int i = 1; i < k-1; i++) {
							 vert += (2*i - k+1)* vert_sum[y][x+i][k]; // integer division!!
						}
						vert += (2*0 - k+1)* vert_sum[y+1][x+0][k-2];
						vert += (2*(k-1) - k+1)* vert_sum[y+1][x+k-1][k-2];
						
						if (hori == 0 && vert == 0) {
							does_this_work = true;
						}					
					}
					if (does_this_work) {
						cout << "Yay!" << endl;
						best = k;
						break; // omg!!
					}
				}
				if (best > 0) {
					break;
				}
			}
			if (best > 0) {
				break;
			}
		}		
		cout << "Case #" << trial << ": ";
		fout << "Case #" << trial << ": ";
		if (best == 0) {
			cout << "IMPOSSIBLE" << endl;
			fout << "IMPOSSIBLE" << endl;
		} else {
			cout << best << endl;
			fout << best << endl;
		}		

	}

}
