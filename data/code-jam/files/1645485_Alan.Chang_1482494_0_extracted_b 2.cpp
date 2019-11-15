#include <iostream>
#include <fstream>
#include <algorithm>
//#include <map>
//#include <string>
//#include <cmath>
#include <vector>

using namespace std;

int num_trials;

class Level {
public:
	int a;
	int b;
	bool is_a_done;
	
	Level(int p_a, int p_b) {
		a = p_a;
		b = p_b;
		is_a_done = false;
	}
};

bool operator< (Level l1, Level l2) {
	if (l1.b < l2.b) {
		return true;
	} else if (l1.b == l2.b && l1.a > l2.a) { // note the > sign
		return true;
	} else {
		return false;
	}
}

int levels;
vector<Level> level; // here we do use zero!

int main(int argc, const char* argv[])  {
    ofstream fout ("b.out");
    ifstream fin ("b.in");

	fin >> num_trials;
	
	for (int trial = 1; trial <= num_trials; trial++) {
		level.clear();
		
		fin >> levels;
		for (int i = 0; i < levels; i++) {
			int a, b;
			fin >> a >> b;
			level.push_back(Level(a, b));
		}
		
		sort (level.begin(), level.end());
		
		cout << "Game " << trial << endl;
		
		for (int i = 0; i < levels; i++) {
			cout << level[i].a << " " <<  level[i].b << endl;
		}
		
		//begin the game!
		
		int stars = 0;
		int plays = 0;
		
		bool made_progress = true;
		
		while(level.size() > 0 && made_progress) {
			made_progress = false;
			
			// going down the list;
			while (level.size() > 0 && stars >= level[0].b) {
				cout << "Whee! I just beat " << level[0].a << " " << level[0].b << " !!!" << endl;
				
				if (level[0].is_a_done == true) {
					stars = stars + 1; 
				} else {
					stars = stars + 2;
				}
				
				plays = plays + 1;
				
				level.erase(level.begin() + 0); // get rid of first element

				
				cout << " Stars: " << stars << endl;
				cout << " Plays: " << plays << endl;
				
				made_progress = true; // we beat something!
			}
			
			if (level.size() == 0) {
				//done!
			} else {
				// going up the list
				int min_b = level[0].b;
				
				int i = level.size() - 1;
				
				// beat enough easy levels so you can go back to hard ones
				while (i >= 0 && stars < min_b) {
					//cout << "Can I beat this level? " << stars << " " << level[i].a << endl;
					if (stars >= level[i].a && ! level[i].is_a_done) {
						
						cout << "Yay! I just beat easy mode of " << level[i].a << " " << level[i].b << " !!!" << endl;
						level[i].is_a_done = true;
						stars = stars + 1;
						plays = plays + 1;
						
										cout << " Stars: " << stars << endl;
				cout << " Plays: " << plays << endl;
				
						made_progress = true; // we beat something!
					}
					i--;
				}
			}
		}
		
		if (made_progress == false) {
			cout << "Ugh... too hard!" << endl;
			fout << "Case #" << trial << ": " << "Too Bad"  << endl;
		} else {
			fout << "Case #" << trial << ": " << plays  << endl;
		}
		
		//string blah;
		//cin >> blah;
	}
	
}
